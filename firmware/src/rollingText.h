#ifndef ROLLINGTEXT_H
#define ROLLINGTEXT_H

#include<stdint.h>
#include"hugeFont.h"
#include"panel.h"

#define MAX_MESSAGE_COLS 2048
#define LETTER_GAP 2

typedef struct {
    uint16_t cols[MAX_MESSAGE_COLS];
    int messageWidth;
} MessageBuffer;

MessageBuffer computeBuffer(const char *text);  // generates frame of entire message (likely larger than panel)
PanelState extractWindow(const MessageBuffer *msg, int offset); // copies scrolling section of the message frame sized to panel


#endif