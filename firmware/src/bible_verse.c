#include "bible_verse.h"
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
#include "rollingText.h"
#include "frames.h"
#include "panel.h"
#include <ctype.h>  // used for lowercase to upercase string char conversion

static const char *TAG = "VERSE";

TaskHandle_t bible_verse_task_handle = NULL;

// static SemaphoreHandle_t verse_mutex;
SemaphoreHandle_t verse_mutex = NULL;

#define VERSE_API_URL "https://bible-api.com/data/web/random"
#define RESPONSE_BUF_SIZE 1024
#define VERSE_START_ROW 13
#define VERSE_CHAR_GAP 1

static char response_buf[RESPONSE_BUF_SIZE];    // static: can't be local since http event handler and fetch verse are seperate function calls but sare this state
static int response_len = 0;

static char current_verse_ref[32];    // "PSA - 115 - 13" — shown in top right area always
static char current_verse_text[512];  // full verse text — used only during full-panel scroll

static esp_err_t http_event_handler(esp_http_client_event_t *evt)       // called for each http chunk arrival
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:        // append to buffer (and don't overflow)
        if(response_len + evt->data_len < RESPONSE_BUF_SIZE-1) {
            memcpy(response_buf + response_len, evt->data, evt->data_len);
            response_len += evt->data_len;
        } else {
            ESP_LOGW(TAG, "Response buffer full, truncating");
        }
        break;
    default:
        break;      // ignore other events
    }
    return ESP_OK;
}

static bool fetch_verse(char *out_buf, size_t out_size)
{
    memset(response_buf, 0, sizeof(response_buf));
    response_len = 0;


    esp_http_client_config_t config = {
        .url = VERSE_API_URL,
        .event_handler = http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if(client == NULL){
        ESP_LOGE(TAG, "Failed to init HTTP Client");
        return false;
    }

    esp_err_t err = esp_http_client_perform(client);       // synchronous: blocks until complete
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

    response_buf[response_len] = '\0';      // nullt-erminated before passing to cJSON
    ESP_LOGI(TAG, "Raw response: %s", response_buf);

    cJSON *json = cJSON_Parse(response_buf);
    if (json == NULL){
        ESP_LOGE(TAG, "Failed to parse JSON");
        return false;
    }

    cJSON *random_verse = cJSON_GetObjectItem(json, "random_verse");
    if(random_verse == NULL){
        ESP_LOGE(TAG, "No 'random_verse' key in response");
        cJSON_Delete(json);
        return false;
    }

    // data extraction
    cJSON *book_id = cJSON_GetObjectItem(random_verse, "book_id");
    cJSON *chapter = cJSON_GetObjectItem(random_verse, "chapter");
    cJSON *verse = cJSON_GetObjectItem(random_verse, "verse");

    // Check types
    if (!cJSON_IsString(book_id) || !cJSON_IsNumber(chapter) || !cJSON_IsNumber(verse)){
        ESP_LOGE(TAG, "Missin or wrong fields in random verse");
        cJSON_Delete(json);
        return false;
    }

    // assemble string to be displayed
    xSemaphoreTake(verse_mutex, portMAX_DELAY); // prenvents value from being read while modified
    snprintf(out_buf, out_size, "%s - %d - %d",
            book_id->valuestring,
            chapter->valueint,
            verse->valueint);
    xSemaphoreGive(verse_mutex);
    
    ESP_LOGI(TAG, "Verse referrence: %s", out_buf);

    // extract verse text
    cJSON *text = cJSON_GetObjectItem(random_verse, "text");
    if (cJSON_IsString(text)) {
        strncpy(current_verse_text, text->valuestring, sizeof(current_verse_text) - 1);
        current_verse_text[sizeof(current_verse_text) - 1] = '\0';
        for (int i = 0; current_verse_text[i]; i++) {
            if (current_verse_text[i] == '\n') current_verse_text[i] = ' ';
        }
        for(int i=0; current_verse_text[i]!='\0'; i++) {
            current_verse_text[i] = toupper((unsigned char)current_verse_text[i]);      // Convert to upper case
        }
        ESP_LOGI(TAG, "Uppercase Verse Contents: %s", current_verse_text);
    }

    cJSON_Delete(json);     // free up heep
    return true;
}

VerseBuffer computeVerseBuffer(){
    VerseBuffer buffer = {0}; // emmpty buffer
    xSemaphoreTake(verse_mutex, portMAX_DELAY); // protects from value being read while updated (if mqtt command received at midnight)
    int colCursor = 0;
    int reflen = strlen(current_verse_ref);
    for(int i=0; i<reflen; i++)  // for each character of the string
    {
        MiniGlyph glyph = fiveXthreeFont[(unsigned char)current_verse_ref[i]];
        for(int x=0; x<glyph.width; x++){ // for each column of the number glyph
            buffer.cols[colCursor] = glyph.cols[x];
            colCursor++;
        }
        colCursor += VERSE_CHAR_GAP;
    }
    xSemaphoreGive(verse_mutex);
    buffer.verseWidth = colCursor;
    return buffer;
}

void addVerseToFrame(PanelState *state, VerseBuffer *verse){
    int start_col = verse->verseWidth;  //start far enough for ref to align on right side, last space will act as buffer
    for(int col=0; col<verse->verseWidth; col++){
    int x = start_col - col;          // start column
    uint8_t colBits = verse->cols[col];
    for(int y = 0; y<5; y++){   // 5 rows tall (time font height)
        if((colBits >> y) & 1){
            state->dots[VERSE_START_ROW - y] |= (1ULL << x);
            }
        }
    }
}

void rollVerseText(char *verseToRoll){
    // Setup
    MessageBuffer buffer = computeBoldBuffer(verseToRoll);
    PanelState localState = initialise_display_map();

    const TickType_t period = pdMS_TO_TICKS(200);
    TickType_t lastWakeTime = xTaskGetTickCount();

    int offset = -2*PANEL_COLS;  // initial offset

    static flipList list;

    while (offset <= buffer.messageWidth)  // 2 panels worth before, 2 panels worth after
    {
        PanelState nextState = extractWindow(&buffer, offset);   // compute frame

        list = compareFrames(&localState, &nextState);
        render_panel(&list);

        localState = nextState;   // refresh state

        offset++;
        vTaskDelayUntil(&lastWakeTime, period);
    }
    
}

void bible_verse_task(void *pvParameter){

    // wait for WiFi before making http request
    ESP_LOGI(TAG, "Waiting for WiFi");
    xEventGroupWaitBits(global_event_group, IS_WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI(TAG, "WiFi connected, fetching verse");


    fetch_verse(current_verse_ref, sizeof(current_verse_ref));      // fetch once on boot
    xEventGroupSetBits(global_event_group, IS_VERSE_READY_BIT);   // signal render loop it can start showing the reference

    while (1)
    {
        // check for a "show verse" notification — but don't block forever waiting for one
        // timeout of 0 means: check right now, continue immediately if nothing pending
        uint32_t notified = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));

        if (notified)
        {
            // CMD_SHOW_VERSE was triggered — scroll full verse text on whole panel
            xSemaphoreTake(panel_mutex, portMAX_DELAY);   // pause render loop
            clearDisplay();
            rollVerseText(current_verse_text); // roll verse contents using full-panel rolling text pipeline
            panel_needs_resync = true;
            xSemaphoreGive(panel_mutex);
        }

        
        time_t now = time(NULL);
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);
        if (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0)      // new verse if midnight
        {
            fetch_verse(current_verse_ref, sizeof(current_verse_ref));
        }

    }
}