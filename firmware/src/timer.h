#ifndef TIMER_H
#define TIMER_H

#include "panel.h"
#include "freertos/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "stdbool.h"

extern TaskHandle_t timer_task_handle;      // so that dispatcher knows who to notify
extern TaskHandle_t alarm_task_handle;

void timer_task(void *pvParameter);
void addTimerToFrame(PanelState *state);     // called once per frame from the main loop
void alarm_task(void *pvParameter);

#endif