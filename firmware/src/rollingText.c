#include "rollingText.h"
#include "hugeFont.h"
#include "frames.h"
#include "panel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include<string.h>

static MessageBuffer buffer;
static PanelState currentState;
static flipList list;

MessageBuffer computeBuffer(const char *txt)
{
    MessageBuffer buffer = {0}; // emmpty buffer
    int colCursor = 0;  // current position within buffer cols
    int txtLength = strlen(txt);
    for(int i=0; i<txtLength; i++){  // for each letter of input string
        Glyph glyph = font[(unsigned char)txt[i]];
        for(int x=0; x<glyph.width; x++){ // for each column of the letter glyph
            buffer.cols[colCursor] = glyph.cols[x];
            colCursor++;
        }
        colCursor += LETTER_GAP;
    }
    buffer.messageWidth = colCursor;
    return buffer;
}

PanelState extractWindow(const MessageBuffer *msg, int offset)
{
    PanelState frame =  initialise_display_map();   // empty PanelState
    for(int x=0; x < 2*PANEL_COLS; x++){ // for each of the 64 PanelState lines
        int msgCol = offset + (2*PANEL_COLS - 1 - x);   // applies offset accounting for panel orientation
        
        uint16_t colBits = 0;
        if(msgCol >= 0 && msgCol < msg->messageWidth){  // handles end of scroll
            colBits = msg->cols[msgCol];
        }

        for (int y = 0; y<PANEL_ROWS; y++){
            if((colBits >> y) & 1){
                frame.dots[y] |=(1ULL << x);    // transpose from vertical uint16_t columns to horizontal uint64_t rows
            }
        }
    }
    return frame;
}

void rolling_text_task(void *pvParameters)
{
    // Setup
    char textToRoll[] = "LET_IT_BE_KNOWN_THAT_CHRIST_IS_KING";
    MessageBuffer buffer = computeBuffer(textToRoll);
    currentState = initialise_display_map();

    const TickType_t period = pdMS_TO_TICKS(250);
    TickType_t lastWakeTime = xTaskGetTickCount();

    int offset = -2*PANEL_COLS;  // initial offset

    while(1){
        PanelState nextState = extractWindow(&buffer, offset);   // compute frame
        flipList list = compareFrames(&currentState, &nextState);
        render_panel(&list);
        currentState = nextState;   // refresh state

        offset+=2;

        if(offset >= buffer.messageWidth+2*PANEL_COLS){
            offset = -2*PANEL_COLS; // reset offset when scroll complete
        }

        vTaskDelayUntil(&lastWakeTime, period);
    }
}