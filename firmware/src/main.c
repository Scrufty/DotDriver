#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "panel.h"
#include "frames.h"
#include "rollingText.h"
#include<string.h>
#include "wifi.h"
#include "ntp.h"
#include <nvs_flash.h>
#include<freertos/event_groups.h>
#include <time.h>

EventGroupHandle_t global_event_group;



static const char *TAG = "MAIN";

static void flicker_animation() // Hardcoded animation: clear all dots on 2 displays, then set them all
{
    // CLEAR DOTS
    selectPanel1();
    esp_rom_delay_us(10);
    clearPanel();
    resetPanel();   // reset counters

    selectPanel2();
    esp_rom_delay_us(10);
    clearPanel();
    resetPanel();   // reset counters

    // SET DOTS
    selectPanel1();
    esp_rom_delay_us(10);
    setPanel();
    resetPanel();

    selectPanel2();
    esp_rom_delay_us(10);
    setPanel();
    resetPanel();
}




/* ------------------------------------------------------------------ */
/* app_main                                                             */
/* ------------------------------------------------------------------ */
void app_main(void)
{
    esp_rom_delay_us(1000000);  // required for serial connection to initialise
    initialiseOutputs();

    esp_err_t err = nvs_flash_init();   // needed to run esp_wifi_init()
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
            nvs_flash_erase();
            nvs_flash_init();
        }

    global_event_group = xEventGroupCreate();   // assignment once declared

    setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);  // timezone
    tzset();
    
    
    // CLEAR DOTS
    clearDisplay();

    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    
    

/*     xTaskCreate(
        rolling_bold_text_task,  // task function rolling_text_task
        "rolling text",     // task name
        16384,               // task-specific stack size in bytes
        NULL,               // any parameters to be passed
        8,                  // priority level (higher = more prefeered)
        NULL                // optional handle for later referencing
    ); */

    xTaskCreate(
        wifi_task,
        "Wi-Fi Keeper",
        configMINIMAL_STACK_SIZE * 3,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        ntp_task,
        "NTP Sync",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        1,
        NULL
    );

    while(1)
    {
        time(&now);                                    // get current timestamp
        localtime_r(&now, &timeinfo);                  // convert to broken-down local time (uses your TZ setting)
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);   // format as a string

        ESP_LOGI(TAG, "current time: %s", strftime_buf);

        vTaskDelay(5000);
    }
}












