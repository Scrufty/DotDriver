#ifndef BIBLE_VERSE_H
#define BIBLE_VERSE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "panel.h"

#define VERSE_COLS 42

typedef struct {
    uint8_t cols[VERSE_COLS];
    int verseWidth;
} VerseBuffer;

extern TaskHandle_t bible_verse_task_handle;

void bible_verse_task(void *pvParameter);
VerseBuffer computeVerseBuffer();
void addVerseToFrame(PanelState *state, VerseBuffer *verse);


#endif