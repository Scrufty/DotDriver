#include "frames.h"
#include "panel.h"
#include <driver/gpio.h>
#include <esp_rom_sys.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

static const char *TAG = "FRAMES";

// NEED TO HARDCODE SOME TEST FRAMES HERE



flipList compareFrames(const PanelState *currentFrame, const PanelState *nextFrame)
{
    ESP_LOGI(TAG, "testing frame compare");
    // start by XORing the 16 uint64_t integers (rows) of current and next frames
    // for each comparison, store the result as a uint64_t so we end up with a changes frame (kinda like a git merge)
    // make a list containing the order of all the dots to be flipped and the target state for each dot (consult nextFrame)
    // write this data to the flipList struct and return it
    PanelState changesFrame = initialise_display_map();    // empty frame
    for(int i=0; i<PANEL_ROWS; i++){
        changesFrame.dots[i] = currentFrame->dots[i] ^ nextFrame->dots[i];   // XOR
    }
    flipList toBeFlipped = {0}; // empty list
    int sizeToFlip = 0;
    for(int i=0; i<(2*PANEL_COLS); i++){    // outer loop: for each column for outer col inner row output (hardware compatible since col advance resets row count)
        for(int j=0; j<PANEL_ROWS; j++){  // for each bit of the changes frame
            if((changesFrame.dots[j]) & (1ULL << i)){ // if j-th bit of row is 1 (dot needs flipping)
                toBeFlipped.list[sizeToFlip].x = i;
                toBeFlipped.list[sizeToFlip].y = j;
                toBeFlipped.list[sizeToFlip].targetState = ((nextFrame->dots[j] >> i) & 1ULL);   // set target state to pixel state in nextFrame
                sizeToFlip++;
            }
        }
    }
    toBeFlipped.count = sizeToFlip;
    return toBeFlipped;
}

void render_panel(flipList *toBeFlipped)
{
    int cursorX=0, cursorY = 0;
    int deltaX=0, deltaY=0;
    int currentPanel = -1;  // force whichever panel to reset on first dot
    for(int i=0; i<toBeFlipped->count; i++){
        int globalX=toBeFlipped->list[i].x;  // row count from 0-63 (accross both panels)
        int y = toBeFlipped->list[i].y;
        int panel = globalX<PANEL_COLS ? 1 : 2; // rows 0-31 -> panel 1, rows 32-63 -> panel 2
        int localX = globalX % PANEL_COLS;  // row count from 0-31 (within a panel)
        
        if(panel != currentPanel){  // we need to change panel (select + reset)
            if(panel == 1){
                selectPanel1();
            }
            else{
                selectPanel2();
            }
            resetPanel();   // reset panel + variable counters
            cursorX = 0;
            cursorY = 0;

            currentPanel = panel;
        }

        deltaX = localX - cursorX;

        if(deltaX != 0){    // we need to change column
            for(int col=0; col<deltaX; col++){
                advanceCol();   // advance columns (must be done before rows, will reset row counter)
            }
            cursorY = 0;    // reset row counter
        }

        deltaY = y - cursorY;   // needs to be computed after column change

        if(deltaY != 0){    // next row (same or different column)
            for(int row=0; row<deltaY; row++){
                advanceRow();
            }
        }

        prepDotState(toBeFlipped->list[i].targetState);
        powerPulse();   // flip dot

        // update cursor
        cursorX = localX;
        cursorY = y;

        ESP_LOGI(TAG, "Finished writing dot %d of %d", i, toBeFlipped->count);
    }
}

// checks for number of changes between current frame and test frame. should return 3 dots (target=1) if current frame is empty
void test_compareFrames(const PanelState *curState)
{
    PanelState testFrame = initialise_display_map(); // start empty
    testFrame.dots[0]  |= (1ULL << 0);   // (x=0,y=0)
    testFrame.dots[3]  |= (1ULL << 5);   // (5,3)
    testFrame.dots[10] |= (1ULL << 40);  // (40,10)

    flipList result = compareFrames(curState, &testFrame);
    // flipList result = compareFrames(&testFrame, curState);   // reverse comparison (switched current/next frames, should produce same positions, opposite target)

        ESP_LOGI(TAG, "compareFrames found %d changes", result.count);
        for (int i = 0; i < result.count; i++) {
            ESP_LOGI(TAG, "Flip #%d: x=%d y=%d target=%d",
                    i, result.list[i].x, result.list[i].y, result.list[i].targetState);
        }
}

// flips through 4 frames on loop
void test_renderFrames(const PanelState *curState, int phase)
{
    //FRAMES
    //frame 1:
    PanelState frame1 = initialise_display_map();
    frame1.dots[9]  |= (1ULL << 13);    //(x=13,y=9)
    frame1.dots[9]  |= (1ULL << 14);    //(x=14,y=9)
    frame1.dots[10] |= (1ULL << 13);    //(x=13,y=10)

    //frame 2:
    PanelState frame2 = initialise_display_map();
    frame2.dots[9]  |= (1ULL << 14);    //(x=14,y=9)
    frame2.dots[9]  |= (1ULL << 15);    //(x=15,y=9)
    frame2.dots[10] |= (1ULL << 15);    //(x=15,y=10)

    //frame 3:
    PanelState frame3 = initialise_display_map();
    frame3.dots[10] |= (1ULL << 15);    //(x=15,y=10)
    frame3.dots[11] |= (1ULL << 14);    //(x=14,y=11)
    frame3.dots[11] |= (1ULL << 15);    //(x=15,y=11)

    //frame 4:
    PanelState frame4 = initialise_display_map();
    frame4.dots[10] |= (1ULL << 13);    //(x=13,y=10)
    frame4.dots[11] |= (1ULL << 13);    //(x=13,y=10)
    frame4.dots[11] |= (1ULL << 14);    //(x=14,y=10)

    PanelState nextState = {0};

    if(phase == 1){
        nextState = frame1;
    } else if (phase == 2){
        nextState = frame2;
    } else if (phase == 3){
        nextState = frame3;
    } else {
        nextState = frame4;
    }

    flipList toBeFlipped = compareFrames(curState, &nextState);

    render_panel(&toBeFlipped);
}