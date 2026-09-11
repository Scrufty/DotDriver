#ifndef COMMANDS_H
#define COMMANDS_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum {          // assigns values 0, 1, 2 → states of CommandType type
    CMD_WATCH_MACHINE,
    CMD_SET_TIMER,
    CMD_SHOW_VERSE,
    CMD_TIMER_EXPIRED
} CommandType;

typedef struct {
    CommandType type;
    union {         // only for commands with a data payload
                    // union = like struct but shared memory since only one payload at a time needed (no wasted memory)
        struct { int machine_id; } watch_machine;
        struct { int duration_seconds;} set_timer;
    } data;
} Command;

extern QueueHandle_t command_queue;     // this variable exists, somewhere else (trust me bro)


#endif