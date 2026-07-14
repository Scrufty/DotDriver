#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "panel.h"
#include "frames.h"
#include "displayTime.h"
#include<string.h>
#include<freertos/event_groups.h>
#include <time.h>

static const char *TAG = "Display-Time";

TimeBuffer computeTimeBuffer()
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    time(&now);                                    // get current timestamp
    localtime_r(&now, &timeinfo);                  // convert to broken-down local time (uses your TZ setting)
    strftime(strftime_buf, sizeof(strftime_buf), "%H:%M", &timeinfo);   // format as a string

    ESP_LOGI(TAG, "current time: %s", strftime_buf);

    TimeBuffer buffer = {0}; // emmpty buffer
    int colCursor = 0;
    for(int i=0; i<(5); i++)  // for each useful character of the time string (hours, ":", mins)
    {
        MiniGlyph glyph = fiveXthreeFont[(unsigned char)strftime_buf[i]];   // 
        for(int x=0; x<glyph.width; x++){ // for each column of the number glyph
            buffer.cols[colCursor] = glyph.cols[x];
            colCursor++;
        }
        colCursor += DIGIT_GAP;
    }
    buffer.timeWidth = colCursor;
    return buffer;
}

void addTimeToFrame(PanelState *state, TimeBuffer *time)
{
    for(int col=0; col<time->timeWidth; col++){
        int x = START_COL - col;
        uint8_t colBits = time->cols[col];
        for(int y = 0; y<5; y++){   // 5 rows tall (MiniGlyph height)
            if((colBits >> y) & 1){
                state->dots[START_ROW - y] |= (1ULL << x);
            }
        }
    }
}