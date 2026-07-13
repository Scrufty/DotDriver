#ifndef GLOBAL_EVENT_GROUP_H
#define GLOBAL_EVENT_GROUP_H

#include <freertos/event_groups.h>

extern EventGroupHandle_t global_event_group;

#define IS_WIFI_CONNECTED_BIT BIT0
#define IS_TIME_SET_BIT BIT1

#endif