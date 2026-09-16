#ifndef GLOBAL_EVENT_GROUP_H
#define GLOBAL_EVENT_GROUP_H

#include <freertos/event_groups.h>
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern EventGroupHandle_t global_event_group;

#define IS_WIFI_CONNECTED_BIT BIT0
#define IS_TIME_SET_BIT BIT1
#define IS_VERSE_READY_BIT BIT2

#endif