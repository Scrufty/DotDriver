#include "dispatcher.h"
#include "commands.h"
#include "timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "bible_verse.h"
#include "washing.h"

static const char *TAG = "dispatcher";

void dispatcher_task(void *pvParameter)
{
    Command cmd;        // will hold reception from queue on each iteration

    while(1)
    {
        // wait for something to arrive in the queue
        xQueueReceive(command_queue, &cmd, portMAX_DELAY);

        switch (cmd.type)
        {
        case CMD_SET_TIMER:
            ESP_LOGI(TAG, "CMD_SET_TIMER received: %d seconds", cmd.data.set_timer.duration_seconds);
            xTaskNotify(                // wake timer task
                timer_task_handle,
                (uint32_t)cmd.data.set_timer.duration_seconds,
                eSetValueWithOverwrite      // overwrite any pending notifications: new timer replaces current one
            );
            break;
        case CMD_WATCH_MACHINE:
            ESP_LOGI(TAG, "CMD_WATCH_MACHINE received: machine %d", cmd.data.watch_machine.machine_id);
            washing_set_watched_machine(cmd.data.watch_machine.machine_id);
            break;

        case CMD_SHOW_VERSE:
            ESP_LOGI(TAG, "CMD_SHOW_VERSE received");
            xTaskNotifyGive(bible_verse_task_handle);
            break;
        case CMD_TIMER_EXPIRED:
            ESP_LOGI(TAG, "Timer finished, running alarm animation");
            xTaskNotify(alarm_task_handle, 0, eNoAction);
            break;
        default:
            ESP_LOGW(TAG, "Unknown command type: %d", cmd.type);
            break;
        }
    }
}