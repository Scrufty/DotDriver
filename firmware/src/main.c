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
#include "displayTime.h"
#include "global_event_group.h"


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

    PanelState cur_state = {0};
    PanelState next_state = {0};

    xEventGroupWaitBits(
        global_event_group,       // event group to watch
        IS_TIME_SET_BIT,          // which bit to wait for
        pdFALSE,                  // don't clear the bit once we see it — leave it set for anyone else checking
        pdTRUE,                   // wait for ALL specified bits (for when waiting for multiple bits)
        portMAX_DELAY             // block indefinitely until the bit is set
    );
    ESP_LOGI(TAG, "Time synced, starting clock display");
    while(1)
    {
        TimeBuffer timeBuffer = computeTimeBuffer();
        TimeBuffer dateBuffer = computeDateBuffer();
        addTimeToFrame(&next_state, &timeBuffer);
        addDateToFrame(&next_state, &dateBuffer);
        flipList list = compareFrames(&cur_state, &next_state);

        render_panel(&list);
        cur_state = next_state;
        next_state = initialise_display_map();

        vTaskDelay(100);
    }
}












