#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_event.h>
#include <esp_netif.h>
#include "wifi.h"
#include "global_event_group.h"

#define SSID            "TP-Link_625A"
#define PASSWORD        "48712064"

static const char *TAG = "Wi-Fi";


static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)    // WiFi callback
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "Connecting to SSID: %s", SSID);
        break;
    
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "Got IP address");
        xEventGroupSetBits(global_event_group, IS_WIFI_CONNECTED_BIT);
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "Lost Connection");
        xEventGroupClearBits(global_event_group, IS_WIFI_CONNECTED_BIT);    // clear flag
        ESP_LOGI(TAG,"WiFi disconnected, reconnecting...");
        wifi_connect();
        break;

    
    default:
        break;
    }
}

void wifi_connect()
{
    xEventGroupClearBits(global_event_group, IS_WIFI_CONNECTED_BIT);
    esp_wifi_connect();
}

void wifi_task(void *pvParameter)
{
    esp_netif_init();   // initialise TCP/IP network stack
    esp_event_loop_create_default();    // starts loop that delivers callback to handler
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();    // creates network interfact for wifi station mode (gets IP, runs DHCP, ...)
    esp_netif_set_hostname(sta_netif, "Phillipe D'ote");
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();    // allocates + initialises ressources for wifi (buffers, task, ...)
    esp_wifi_init(&wifi_initiation);
    wifi_config_t wifi_configuration = {   // set preferences (selected security: WPA2_PSK)
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_start();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);     // subscribe: callback if any wifi event
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);    // callback if GOT_IP event

    wifi_connect();

    vTaskDelete(NULL);   // this task's job is done; handler manages reconnects from here

}