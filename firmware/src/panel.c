
#include <panel.h>
#include <driver/gpio.h>
#include <esp_rom_sys.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

#define COL_LED CONFIG_COL_LED
#define ROW_LED CONFIG_ROW_LED
#define PULSE_LED CONFIG_PULSE_LED
#define SEL_PANEL_1 CONFIG_SEL_PANEL_1
#define SEL_PANEL_2 CONFIG_SEL_PANEL_2
#define SEL_PANEL_3 CONFIG_SEL_PANEL_3
#define SEL_PANEL_4 CONFIG_SEL_PANEL_4
#define SET_UNSET CONFIG_SET_UNSET
#define COL_ADV CONFIG_COL_ADV
#define ROW_ADV CONFIG_ROW_ADV
#define PULSE_SIG CONFIG_PULSE_SIG
#define PANEL_RESET CONFIG_PANEL_RESET

#define PULSE_US            200

static const char *TAG = "panel";


void initialiseOutputs() {
    ESP_LOGI(TAG, "Initialising Outputs");
    gpio_reset_pin(COL_ADV);
    gpio_reset_pin(COL_LED);
    gpio_reset_pin(ROW_ADV);
    gpio_reset_pin(ROW_LED);
    gpio_reset_pin(PULSE_SIG);
    gpio_reset_pin(PULSE_LED);
    gpio_reset_pin(SEL_PANEL_1);
    gpio_reset_pin(SEL_PANEL_2);
    gpio_reset_pin(SEL_PANEL_3);
    gpio_reset_pin(SEL_PANEL_4);
    gpio_reset_pin(SET_UNSET);
    gpio_reset_pin(PANEL_RESET);
    // set to push/pull output
    gpio_set_direction(COL_ADV, GPIO_MODE_OUTPUT);
    gpio_set_direction(COL_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(ROW_ADV, GPIO_MODE_OUTPUT);
    gpio_set_direction(ROW_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(PULSE_SIG, GPIO_MODE_OUTPUT);
    gpio_set_direction(PULSE_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEL_PANEL_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEL_PANEL_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEL_PANEL_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEL_PANEL_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(SET_UNSET, GPIO_MODE_OUTPUT);
    gpio_set_direction(PANEL_RESET, GPIO_MODE_OUTPUT);
}

PanelState initialise_display_map() {
    PanelState state = {0};
    return state;
}

void advanceCol() {
    gpio_set_level(COL_ADV, 1);
    gpio_set_level(COL_LED, 1);
    esp_rom_delay_us(10);   // 0.1ms
    gpio_set_level(COL_ADV, 0);
    gpio_set_level(COL_LED, 0);
    esp_rom_delay_us(10);
}

void advanceRow() {
    gpio_set_level(ROW_ADV, 1);
    gpio_set_level(ROW_LED, 1);
    esp_rom_delay_us(10);   // 0.1ms
    gpio_set_level(ROW_ADV, 0);
    gpio_set_level(ROW_LED, 0);
    esp_rom_delay_us(10);
}

void resetPanel() {
    advanceCol();
    gpio_set_level(PANEL_RESET, 1);
    esp_rom_delay_us(1000); // 1ms
    gpio_set_level(PANEL_RESET, 0);
    for(int i=0; i<31; i++){    // not sure why panel needs this but initialises to 1 rather than 0 otherwise
        advanceCol();
    }
}

void goToDot(uint8_t x, uint8_t y) {    // NEED TO ADD AUTOMATED PANEL SELECT
    resetPanel();   // clear counters
    for(uint8_t i=0; i<x; i++) {    // columns before row (col adv reset row count)
        advanceCol();
        esp_rom_delay_us(10);
    }

    for(uint8_t i=0; i<y; i++) {
        advanceRow();
        esp_rom_delay_us(10);
    }
}

void selectPanel1() {
    gpio_set_level(SEL_PANEL_1, 1);
    gpio_set_level(SEL_PANEL_2, 0);
    gpio_set_level(SEL_PANEL_3, 0);
    gpio_set_level(SEL_PANEL_4, 0);
}

void selectPanel2() {
    gpio_set_level(SEL_PANEL_1, 0);
    gpio_set_level(SEL_PANEL_2, 1);
    gpio_set_level(SEL_PANEL_3, 0);
    gpio_set_level(SEL_PANEL_4, 0);
}

void selectPanel3() {
    gpio_set_level(SEL_PANEL_1, 0);
    gpio_set_level(SEL_PANEL_2, 0);
    gpio_set_level(SEL_PANEL_3, 1);
    gpio_set_level(SEL_PANEL_4, 0);
}

void selectPanel4() {
    gpio_set_level(SEL_PANEL_1, 0);
    gpio_set_level(SEL_PANEL_2, 0);
    gpio_set_level(SEL_PANEL_3, 0);
    gpio_set_level(SEL_PANEL_4, 1);
}

void prepDotState(bool state) {
    gpio_set_level(SET_UNSET, state);
}

void powerPulse() {
    gpio_set_level(PULSE_SIG, 1); // send pulse
    gpio_set_level(PULSE_LED, 1);
    esp_rom_delay_us(PULSE_US);
    gpio_set_level(PULSE_SIG, 0);
    gpio_set_level(PULSE_LED, 0);
    esp_rom_delay_us(10);
}

void test() {
    gpio_set_level(SEL_PANEL_1, 1);
    gpio_set_level(SEL_PANEL_2, 1);
    gpio_set_level(SEL_PANEL_3, 1);
    gpio_set_level(SEL_PANEL_4, 1);
    gpio_set_level(COL_ADV, 1);
    gpio_set_level(COL_LED, 1);
    gpio_set_level(PANEL_RESET , 1);
    gpio_set_level(SET_UNSET, 1);
    gpio_set_level(ROW_ADV, 1);
    gpio_set_level(ROW_LED, 1);
    gpio_set_level(PULSE_SIG, 1);
    gpio_set_level(PULSE_LED, 1);
    ESP_LOGI(TAG, "ON");

    vTaskDelay(3000/portTICK_PERIOD_MS);

    gpio_set_level(SEL_PANEL_1, 0);
    gpio_set_level(SEL_PANEL_2, 0);
    gpio_set_level(SEL_PANEL_3, 0);
    gpio_set_level(SEL_PANEL_4, 0);
    gpio_set_level(COL_ADV, 0);
    gpio_set_level(COL_LED, 0);
    gpio_set_level(PANEL_RESET , 0);
    gpio_set_level(SET_UNSET, 0);
    gpio_set_level(ROW_ADV, 0);
    gpio_set_level(ROW_LED, 0);
    gpio_set_level(PULSE_SIG, 0);
    gpio_set_level(PULSE_LED, 0);
    ESP_LOGI(TAG, "OFF");

    vTaskDelay(2000/portTICK_PERIOD_MS);
}

void clear_row(void)
{
    ESP_LOGI(TAG, "clearing column");
//    resetPanel();   // clear counters
    gpio_set_level(CONFIG_COL_ADV, 1);
    esp_rom_delay_us(10);
    gpio_set_level(CONFIG_COL_ADV, 0);


    gpio_set_level(CONFIG_SET_UNSET, 0); // unset pins
    for(int i=0; i<16; i++){
        powerPulse();   // send pulse

        gpio_set_level(CONFIG_ROW_ADV, 1);
        esp_rom_delay_us(10);
        gpio_set_level(CONFIG_ROW_ADV, 0);
        esp_rom_delay_us(10);
    }

}

void set_row(void)
{
    ESP_LOGI(TAG, "setting column");
//    resetPanel();   // clear counters
    advanceCol();
    
    prepDotState(1); // set pins
    for(int i=0; i<16; i++){
        powerPulse();   // send pulse

        advanceRow();
    }

}

void clearPanel()
{
    resetPanel();   // reset counters
    for(int i=0; i<32; i++){
        clear_row();
    }
    esp_rom_delay_us(10);
}

void setPanel()
{
    resetPanel();
    for(int i=0; i<32; i++){
        set_row();
    }
    esp_rom_delay_us(10);
}