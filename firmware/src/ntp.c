#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<esp_netif_sntp.h>
#include<esp_log.h>
#include"global_event_group.h"
#include"ntp.h"

static const char *TAG = "NTP";

void ntp_task(void *pvParameter)
{
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");

    while(true)
    {
        xEventGroupWaitBits(global_event_group, IS_WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY); //wait till connected flag set
        
        esp_netif_sntp_init(&config);
        esp_err_t err = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)); // 10sec timeout
        
        if(err == ESP_OK){
            ESP_LOGI(TAG,"Time Synced, next sync in 24 hours");
            xEventGroupSetBits(global_event_group, IS_TIME_SET_BIT);
        } else {
            ESP_LOGI(TAG, "Failed to sync time");
        }

        vTaskDelay(pdMS_TO_TICKS(24*60*60*1000UL)); // wait 24 hours
    }

}