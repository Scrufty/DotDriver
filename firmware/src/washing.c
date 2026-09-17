#include "washing.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "cJSON.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdio.h>
#include "panel.h"
#include "global_event_group.h"
#include <time.h>
#include <stdbool.h>
#include "hugeFont.h"

static const char *TAG = "WASHING";

static int watched_machine_id = 0;       // 0 = not watching anything
static bool machine_seen_running = false; // true if has status has been !=1 since started watching
static bool machine_done = false;         // was running, now free
static char washing_string[32];

SemaphoreHandle_t washing_mutex = NULL;
TaskHandle_t washing_task_handle = NULL;


#define WASHING_API_URL "https://status.wi-line.fr/update_machine_ext.php"
#define WASHING_RESPONSE_BUF_SIZE 4096
#define WASHING_CHAR_GAP 1
#define WASHING_START_ROW 5

static char washing_response_buf[WASHING_RESPONSE_BUF_SIZE];    // static: can't be local since http event handler and fetch verse are seperate function calls but sare this state
static int washing_response_len = 0;

static esp_err_t washing_http_event_handler(esp_http_client_event_t *evt)       // called for each http chunk arrival
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:        // append to buffer (and don't overflow)
        if(washing_response_len + evt->data_len < WASHING_RESPONSE_BUF_SIZE-1) {
            memcpy(washing_response_buf + washing_response_len, evt->data, evt->data_len);
            washing_response_len += evt->data_len;
        } else {
            ESP_LOGW(TAG, "Response buffer full, truncating");
        }
        break;
    default:
        break;      // ignore other events
    }
    return ESP_OK;
}

bool checkWashing(int watched_id, int *out_status){
    memset(washing_response_buf, 0, sizeof(washing_response_buf));     // reset buffer
    washing_response_len = 0;

    esp_http_client_config_t config = {
    .url = WASHING_API_URL,
    .method = HTTP_METHOD_POST,
    .event_handler = washing_http_event_handler,   // != from verse handler
    .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if(client == NULL){
        ESP_LOGE(TAG, "Failed to init HTTP client");
        return false;
    }

    esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
    const char *post_body = "action=READ_LIST_STATUS&serial_centrale=3e60d17bbf82f5e7e24fdce3fc73983f";
    esp_http_client_set_post_field(client, post_body, strlen(post_body));
    
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if(err != ESP_OK) {
        ESP_LOGE(TAG,"HTTP GET failed: %s", esp_err_to_name(err));
        return false;
    }
    if(status != 200) {
        ESP_LOGE(TAG, "Unexpected HTTP status: %d", status);
        return false;
    }

    washing_response_buf[washing_response_len] = '\0';      // nullt-erminated before passing to cJSON
    
    cJSON *json = cJSON_Parse(washing_response_buf);
    if(json == NULL){
        ESP_LOGE(TAG, "Failed to parse JSON");
        return false;
    }

    bool found = false;
    int array_size = cJSON_GetArraySize(json);
    for(int i=0; i<array_size; i++){
        cJSON *machine = cJSON_GetArrayItem(json, i);
        cJSON *selecteur = cJSON_GetObjectItem(machine, "selecteur_machine");
        cJSON *mstatus = cJSON_GetObjectItem(machine, "status");


        if(cJSON_IsNumber(selecteur)&& cJSON_IsNumber(mstatus) && selecteur->valueint == watched_id){
            *out_status = mstatus->valueint;
            cJSON_Delete(json);
            found = true;
            ESP_LOGI(TAG, "Found machine: #%d, status: %d", selecteur->valueint, mstatus->valueint);
            return found;
        }
    }

    cJSON_Delete(json);     // free up heep
    return false;
}

void washing_set_watched_machine(int machine_id){       // specifies which machine is watched
    xSemaphoreTake(washing_mutex, portMAX_DELAY);
    watched_machine_id = machine_id;
    machine_seen_running = false;
    machine_done = false;
    xSemaphoreGive(washing_mutex);

    if(washing_task_handle != NULL){
        xTaskNotifyGive(washing_task_handle);
    }
}

WashingBuffer computeWashingBuffer(){       // generates buffer from washing string
    WashingBuffer buffer = {0}; // emmpty buffer
    xSemaphoreTake(washing_mutex, portMAX_DELAY); // protects from value being read while updated (if mqtt command received at midnight)
    int colCursor = 0;
    int washlen = strlen(washing_string);
    for(int i=0; i<washlen; i++)  // for each character of the string
    {
        MiniGlyph glyph = fourXthreeFont[(unsigned char)washing_string[i]];
        for(int x=0; x<glyph.width; x++){ // for each column of the glyph
            buffer.cols[colCursor] = glyph.cols[x];
            colCursor++;
        }
        colCursor += WASHING_CHAR_GAP;
    }
    xSemaphoreGive(washing_mutex);
    buffer.washingWidth = colCursor;
    return buffer;
}

void addWashingToFrame(PanelState *state, WashingBuffer *buf){
    int start_col = buf->washingWidth;  //start far enough for ref to align on right side, last space will act as buffer
    for(int col=0; col<buf->washingWidth; col++){
    int x = start_col - col;          // start column
    uint8_t colBits = buf->cols[col];
    for(int y = 0; y<4; y++){   // 4 rows tall (washing font height)
        if((colBits >> y) & 1){
            state->dots[WASHING_START_ROW - y] |= (1ULL << x);
            }
        }
    }
}

void washing_task (void *pvParameter){
    
    ESP_LOGI(TAG, "Waiting for WiFi");      // wait for WiFi before making http request
    xEventGroupWaitBits(global_event_group, IS_WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI(TAG, "WiFi connected, checking washing machines");
    
    while(1){
        int local_id;
        xSemaphoreTake(washing_mutex, portMAX_DELAY);
        local_id = watched_machine_id;
        xSemaphoreGive(washing_mutex);

        if(local_id != 0){
            int status;
            if(checkWashing(local_id, &status))
            {
                xSemaphoreTake(washing_mutex, portMAX_DELAY);
                if(watched_machine_id == local_id){
                    if(status != 1){                    // machine in use
                        machine_seen_running = true;
                        machine_done = false;
                    } else if(machine_seen_running){    // machine free
                        machine_done = true;
                    }
                }
                xSemaphoreGive(washing_mutex);
            } else {
                ESP_LOGI(TAG, "Failed to fetch washing data");
            }
        }       // else: machine id 0 (reset command) -> no running/done booleans to change, just display rest pattern

        xSemaphoreTake(washing_mutex, portMAX_DELAY);
        if(watched_machine_id == 0){                                                        // Resting state (no machine with id 0)
            snprintf(washing_string, sizeof(washing_string), "R");
        } else if(machine_done){
            snprintf(washing_string, sizeof(washing_string), "%d D", watched_machine_id);   // Watched machine finished cycle
        } else if(machine_seen_running){
            snprintf(washing_string, sizeof(washing_string), "%d W", watched_machine_id);  // Watched machine still running
        } else{
            // snprintf(washing_string, sizeof(washing_string), "R");  // machine subbed to isn't running yet: stay in rest pattern
            snprintf(washing_string, sizeof(washing_string), "%d N", watched_machine_id);  // machine subbed to isn't running yet: test behaviour: display "%id unused"
        }
        xSemaphoreGive(washing_mutex);

        // sleep 60s, but wake early if the watched machine changes
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(60000));
    }
    
}