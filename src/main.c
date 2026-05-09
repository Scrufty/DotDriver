#include <esp_log.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <sdkconfig.h>

#define COL CONFIG_COL_LED
#define ROW CONFIG_ROW_LED
#define PULSE CONFIG_PULSE_LED

static const char *TAG = "example";

static uint8_t phase = 0;

static void drive_leds(int phaseNB) {
    if(phaseNB == 0){
        gpio_set_level(COL, 1);
        gpio_set_level(ROW, 0);
        gpio_set_level(PULSE, 0);
    } else if (phaseNB == 1)
    {
        gpio_set_level(COL, 0);
        gpio_set_level(ROW, 1);
        gpio_set_level(PULSE, 0);
    } else
    {
        gpio_set_level(COL, 0);
        gpio_set_level(ROW, 0);
        gpio_set_level(PULSE, 1);
    }
}

static void initialise_leds(void){
    ESP_LOGI(TAG, "Initialising LED pins");
    gpio_reset_pin(COL);
    gpio_reset_pin(ROW);
    gpio_reset_pin(PULSE);
    // set to push/pull output
    gpio_set_direction(COL, GPIO_MODE_OUTPUT);
    gpio_set_direction(ROW, GPIO_MODE_OUTPUT);
    gpio_set_direction(PULSE, GPIO_MODE_OUTPUT);
}


void app_main() {
    initialise_leds();

    while(1) {
        while(phase < 3) {
            ESP_LOGI(TAG, "Current Phase: %d", phase);
            drive_leds(phase);
            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            phase ++;
        }
        phase = 0;  //reset counter
    }
}