#include "dispatcher.h"
#include "commands.h"
#include "timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "timer.h"

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
            // TODO: add machine_id to the shared watched_machines array, washing_task will need to poll the API and check this list
            break;

        case CMD_SHOW_VERSE:
            ESP_LOGI(TAG, "CMD_SHOW_VERSE received");
            // TODO: notify verse_task will need to fetch and display the verse of the day via HTTP, then feed into rolling text pipeline
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