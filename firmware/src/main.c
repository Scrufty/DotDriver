#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "panel.h"
#include "frames.h"
#include "rollingText.h"
#include<string.h>

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
    
    // CLEAR DOTS
    clearDisplay();

    
    ESP_LOGI(TAG, "entering loop");

    xTaskCreate(
        rolling_bold_text_task,  // task function rolling_text_task
        "rolling text",     // task name
        16384,               // task-specific stack size in bytes
        NULL,               // any parameters to be passed
        8,                  // priority level (higher = more prefeered)
        NULL                // optional handle for later referencing
    );

}












