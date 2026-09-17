#ifndef WASHING_H
#define WASHING_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "panel.h"

#define WASHING_COLS 42

typedef struct {
    uint8_t cols[WASHING_COLS];
    int washingWidth;
} WashingBuffer;



extern TaskHandle_t washing_task_handle;
extern SemaphoreHandle_t washing_mutex;


bool checkWashing(int watched_id, int *out_status);
void washing_set_watched_machine(int machine_id);
void washing_task(void *pvParameter);
WashingBuffer computeWashingBuffer();
void addWashingToFrame(PanelState *state, WashingBuffer *verse);


#endif