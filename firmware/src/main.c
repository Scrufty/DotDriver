#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "panel.h"
#include "frames.h"
#include "rollingText.h"
#include <string.h>
#include "wifi.h"
#include "ntp.h"
#include <nvs_flash.h>
#include <freertos/event_groups.h>
#include <time.h>
#include "displayTime.h"
#include "global_event_group.h"
#include "mqtt.h"
#include "commands.h"
#include "timer.h"
#include "dispatcher.h"
#include "bible_verse.h"
#include "washing.h"


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
/* app_main                                                           */
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

    command_queue = xQueueCreate(10, sizeof(Command)); // currently set to 10 commands

    

    setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);  // timezone for clock
    tzset();
    
    panel_mutex = xSemaphoreCreateMutex();
    verse_mutex = xSemaphoreCreateMutex();
    washing_mutex = xSemaphoreCreateMutex();


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

    xTaskCreate(
        mqtt_task,
        "MQTT",
        configMINIMAL_STACK_SIZE * 4, // higher for TLS handshake
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        timer_task,
        "Timer",
        4096,
        NULL,
        3,
        &timer_task_handle
    );

    xTaskCreate(
        dispatcher_task,
        "Dispatcher",
        4096,
        NULL,
        2,              // slightly higher priority than WiFi
        NULL            // no handle since task never notified by anything else
    );

    xTaskCreate(
        alarm_task,
        "Timer_Alarm",
        16384,
        NULL,
        2,              // slightly higher priority than WiFi
        &alarm_task_handle
    );

    xTaskCreate(
        bible_verse_task,
        "Bible_Verse",
        32768,       // was 16384, might need more
        NULL,
        2,
        &bible_verse_task_handle
    );

        xTaskCreate(
        washing_task,
        "Washing",
        4096,
        NULL,
        2,
        &washing_task_handle
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
        VerseBuffer verseBuffer = computeVerseBuffer();
        WashingBuffer washingBuffer = computeWashingBuffer();
        int bar = compute_day_progress_bar();

        next_state = initialise_display_map();
        addTimeToFrame(&next_state, &timeBuffer);
        addDateToFrame(&next_state, &dateBuffer);
        addTimerToFrame(&next_state);
        addDayProgressToFrame(&next_state, bar);
        addVerseToFrame(&next_state, &verseBuffer);
        addWashingToFrame(&next_state, &washingBuffer);

        xSemaphoreTake(panel_mutex, portMAX_DELAY);
        if(panel_needs_resync){
            cur_state = (PanelState){0};      //blank canvas
            panel_needs_resync = false;
        }
        flipList list = compareFrames(&cur_state, &next_state);
        render_panel(&list);
        xSemaphoreGive(panel_mutex);

        cur_state = next_state;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}












