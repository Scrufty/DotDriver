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

    // char textToRoll[] = "LET_IT_BE_KNOWN_THAT_CHRIST_IS_KING";
    char textToRoll[] = "HI!";
    MessageBuffer buffer = computeBuffer(textToRoll);
    ESP_LOGI(TAG, "entering loop");

    while (1) {
        for(int offset = -2*PANEL_COLS; offset<buffer.messageWidth + 2*PANEL_COLS; offset++){
            PanelState nextState = extractWindow(&buffer, offset);   // compute frame
            flipList list = compareFrames(&currentState, &nextState);
            render_panel(&list);
            currentState = nextState;   // refresh state
            esp_rom_delay_us(50000);
        }
    

        // flicker_animation();

/*         test_renderFrames(&currentState, 1);
        currentState = initialise_display_map();
        currentState.dots[9]  |= (1ULL << (13+18));    //(x=13,y=9)
        currentState.dots[9]  |= (1ULL << (14+18));    //(x=14,y=9)
        currentState.dots[10] |= (1ULL << (13+18));    //(x=13,y=10)
        esp_rom_delay_us(80000);

        test_renderFrames(&currentState, 2);
        currentState = initialise_display_map();
        currentState.dots[9]  |= (1ULL << (14+18));    //(x=14,y=9)
        currentState.dots[9]  |= (1ULL << (15+18));    //(x=15,y=9)
        currentState.dots[10] |= (1ULL << (15+18));    //(x=15,y=10)
        esp_rom_delay_us(80000);

        test_renderFrames(&currentState, 3);
        currentState = initialise_display_map();
        currentState.dots[10] |= (1ULL << (15+18));    //(x=15,y=10)
        currentState.dots[11] |= (1ULL << (14+18));    //(x=14,y=11)
        currentState.dots[11] |= (1ULL << (15+18));    //(x=15,y=11)
        esp_rom_delay_us(80000);

        test_renderFrames(&currentState, 4);
        currentState = initialise_display_map();
        currentState.dots[10] |= (1ULL << (13+18));    //(x=13,y=10)
        currentState.dots[11] |= (1ULL << (13+18));    //(x=13,y=10)
        currentState.dots[11] |= (1ULL << (14+18));    //(x=14,y=10)
        esp_rom_delay_us(80000); */

    }
}












