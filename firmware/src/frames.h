
#ifndef FRAMES_H
#define FRAMES_H

#include"panel.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t x;
    uint8_t y;
    bool targetState;
} flipItem;

typedef struct {
    flipItem list[2 * PANEL_ROWS * PANEL_COLS]; // big enough for 2 pannels
    int count;   // how far to read the list (unlikely to be full)
} flipList;

flipList compareFrames(const PanelState *currentFrame, const PanelState *nextFrame);
void render_panel(flipList *toBeFlipped);
void test_compareFrames(const PanelState *curState);
void test_renderFrames(const PanelState *curState, int phase);

#endif