#include "timer.h"
#include "panel.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <time.h>
#include <stdio.h>
#include "commands.h"
#include "esp_timer.h"


static const char *TAG = "TIMER";

static SemaphoreHandle_t timer_mutex;
static bool timer_running = false;
static int64_t timer_start_us;
static int64_t timer_duration_us;

TaskHandle_t timer_task_handle = NULL;
TaskHandle_t alarm_task_handle = NULL;

PanelState compute_timer_buffer(int dots_to_light)
{
    PanelState timer_buf = initialise_display_map();
    int count = 0;

    for(int y=0; y<=15 && count<dots_to_light; y++, count++){         // first segment: right edge
        timer_buf.dots[y] |= (1ULL << 0);
    }

    for(int x=1; x<=63 && count<dots_to_light; x++, count++){         // second segment: top edge
        timer_buf.dots[15] |= (1ULL << x);
    }

    for(int y=14; y>=0 && count<dots_to_light; y--, count++){         // third segment: left edge
        timer_buf.dots[y] |= (1ULL << 63);
    }

    for(int x=62; x>=0 && count<dots_to_light; x--, count++){         // forth segment: bottom edge
        timer_buf.dots[0] |= (1ULL << x);
    }

    return timer_buf;
}

void timer_task(void *pvParameter)
{
    timer_mutex = xSemaphoreCreateMutex();

    while(1) {
        uint32_t duration_seconds;
        xTaskNotifyWait(0, 0, &duration_seconds, portMAX_DELAY);    // wait till notified
        
        xSemaphoreTake(timer_mutex, portMAX_DELAY);
        timer_running = true;
        timer_start_us = esp_timer_get_time();
        timer_duration_us = (int64_t)duration_seconds * 1000000LL;
        xSemaphoreGive(timer_mutex);

        ESP_LOGI(TAG, "Timer started, %lu seconds", duration_seconds);
    }
}

void addTimerToFrame(PanelState *state){
    xSemaphoreTake(timer_mutex, portMAX_DELAY);
    bool running = timer_running;
    int64_t elapsed_us = esp_timer_get_time() - timer_start_us;
    int64_t duration_us = timer_duration_us;
    if(running && elapsed_us >= duration_us){
        timer_running = false;
        running = false;
        Command cmd ={ .type = CMD_TIMER_EXPIRED};      // tell dispatcher timer is finished
        xQueueSend(command_queue, &cmd, 0);
    }
    xSemaphoreGive(timer_mutex);

    if(!running){
        // all perimeter active
        PanelState full = compute_timer_buffer(156);
        for(int y = 0; y<PANEL_ROWS;y++){
            state->dots[y] |= full.dots[y];
        }
        return;
    }

    int dots_to_light = (int)(156.0 * (double)elapsed_us/(double)duration_us);
    PanelState timer_buf = compute_timer_buffer(dots_to_light);
    for(int y=0; y< PANEL_ROWS; y++){
        state->dots[y] |= timer_buf.dots[y];
    }
    
}

void alarm_task(void *pvParameter)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);        // wait until notified to run

        xSemaphoreTake(panel_mutex, portMAX_DELAY);
        clearDisplay();
        setDisplay();
        clearDisplay();
        panel_needs_resync = true;      // cur panel no longer matches irl panel state
        xSemaphoreGive(panel_mutex);
    }
}