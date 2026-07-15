#ifndef DISPLAYTIME_H
#define DISPLAYTIME_H

#include<stdint.h>
#include"hugeFont.h"
#include"panel.h"

#define TIME_COLS 18
#define DIGIT_GAP 1

#define START_COL 61    // left-most column of first digit on panel
#define TIME_START_ROW 13     // 
#define DATE_START_ROW 5     // 

typedef struct {
    uint16_t cols[TIME_COLS];
    int timeWidth;
} TimeBuffer;

TimeBuffer computeTimeBuffer();  // generates frame region for time
TimeBuffer computeDateBuffer();  // same for date
void addTimeToFrame(PanelState *state, TimeBuffer *time); // adds time info to full sized frame
void addDateToFrame(PanelState *state, TimeBuffer *time); // same for date


#endif