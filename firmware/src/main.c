#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "panel.h"
#include "frames.h"

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
    esp_rom_delay_us(1000000);
    initialiseOutputs();
    ESP_LOGI(TAG, "testing frame compare");
    //test_compareFrames(&curState);
    PanelState currentState = initialise_display_map();
    
    // CLEAR DOTS
    selectPanel1();
    esp_rom_delay_us(10);
    clearPanel();
    resetPanel();   // reset counters

    selectPanel2();
    esp_rom_delay_us(10);
    clearPanel();
    resetPanel();   // reset counters
    esp_rom_delay_us(1000000);

    while (1) {
        // flicker_animation();

        test_renderFrames(&currentState, 1);
        currentState = initialise_display_map();
        currentState.dots[9]  |= (1ULL << 13);    //(x=13,y=9)
        currentState.dots[9]  |= (1ULL << 14);    //(x=14,y=9)
        currentState.dots[10] |= (1ULL << 13);    //(x=13,y=10)
        esp_rom_delay_us(1000000);

        test_renderFrames(&currentState, 2);
        currentState = initialise_display_map();
        currentState.dots[9]  |= (1ULL << 14);    //(x=14,y=9)
        currentState.dots[9]  |= (1ULL << 15);    //(x=15,y=9)
        currentState.dots[10] |= (1ULL << 15);    //(x=15,y=10)
        esp_rom_delay_us(1000000);

        test_renderFrames(&currentState, 3);
        currentState = initialise_display_map();
        currentState.dots[10] |= (1ULL << 15);    //(x=15,y=10)
        currentState.dots[11] |= (1ULL << 14);    //(x=14,y=11)
        currentState.dots[11] |= (1ULL << 15);    //(x=15,y=11)
        esp_rom_delay_us(1000000);

        test_renderFrames(&currentState, 4);
        currentState = initialise_display_map();
        currentState.dots[10] |= (1ULL << 13);    //(x=13,y=10)
        currentState.dots[11] |= (1ULL << 13);    //(x=13,y=10)
        currentState.dots[11] |= (1ULL << 14);    //(x=14,y=10)
        esp_rom_delay_us(1000000);

    }
}












