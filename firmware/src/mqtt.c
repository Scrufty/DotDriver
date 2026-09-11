#include "mqtt_client.h"
#include "esp_crt_bundle.h"
#include <esp_event.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include "global_event_group.h"
#include "mqtt.h"
#include "cJSON.h"
#include "commands.h"


static const char *TAG = "mqtt";

#define MQTT_BROKER_URI "PLACEHOLDER"
#define MQTT_USERNAME   "PLACEHOLDER"
#define MQTT_PASSWORD   "PLACEHOLDER"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT connected");
            esp_mqtt_client_subscribe(client, "flipdot/commands", 1);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT disconnected");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);
            char payload[128];
            // use whichever is smaller: message length or max capacity
            int len = event->data_len < sizeof(payload) - 1 ? event->data_len: sizeof(payload)-1;
            memcpy(payload, event->data, len);
            payload[len] = '\0';    // null terminator (why 1 is subtracted above), required for recognition as parsable C-string

            cJSON *json = cJSON_Parse(payload);
            if(json == NULL)
            {
                ESP_LOGE(TAG, "Failed to parse Json command");
                break;
            }

            cJSON *type = cJSON_GetObjectItem(json, "type");
            if(cJSON_IsString(type)) {                              // avoid crash due to type mismatch
                Command cmd = {0};
                if(strcmp(type->valuestring,"watch_machine") == 0) {                // washing machine command  (0 = no differences found)
                    cJSON *id = cJSON_GetObjectItem(json, "id");
                    if (cJSON_IsNumber(id)) {
                        cmd.type = CMD_WATCH_MACHINE;
                        cmd.data.watch_machine.machine_id = id->valueint;
                        xQueueSend(command_queue, &cmd, 0);
                    }
                } else if(strcmp(type->valuestring, "set_timer") == 0){             // set timer command
                    cJSON *duration =cJSON_GetObjectItem(json, "duration");
                    if (cJSON_IsNumber(duration)){
                        cmd.type = CMD_SET_TIMER;
                        cmd.data.set_timer.duration_seconds = duration-> valueint;
                        xQueueSend(command_queue, &cmd, 0);
                    }
                } else if(strcmp(type->valuestring, "show_verse") == 0){
                    cmd.type = CMD_SHOW_VERSE;
                    xQueueSend(command_queue, &cmd, 0);
                }
            }
            cJSON_Delete(json);     // free up space when done
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT error");
            break;
        default:
            break;
    }
}

void mqtt_task(void *pvParameter)
{
    ESP_LOGI(TAG, "WAITING FOR WIFI");
    xEventGroupWaitBits(global_event_group, IS_WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address.uri = MQTT_BROKER_URI,
            .verification.crt_bundle_attach = esp_crt_bundle_attach,
        },
        .credentials = {
            .username = MQTT_USERNAME,
            .authentication.password = MQTT_PASSWORD,
        },
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    vTaskDelete(NULL);  // can get rid of this task since the client runs its own internal task
}