#ifndef DISPLAYTIME_H
#define DISPLAYTIME_H

#include<stdint.h>
#include"hugeFont.h"
#include"panel.h"

#define TIME_COLS 18
#define DIGIT_GAP 1

#define START_COL 61    // left-most column of first digit on panel
#define START_ROW 13     // 


typedef struct {
    uint16_t cols[TIME_COLS];
    int timeWidth;
} TimeBuffer;

TimeBuffer computeTimeBuffer();  // generates 17 by 8 frame of current time
void addTimeToFrame(PanelState *state, TimeBuffer *time); // copies scrolling section of the message frame sized to panel


#endif