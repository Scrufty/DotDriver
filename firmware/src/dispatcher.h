#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void dispatcher_task(void *pvParameter);

#endif