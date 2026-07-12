
#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>
#include <stdbool.h>


#define PANEL_ROWS         16         
#define PANEL_COLS         32

typedef struct {
    uint64_t dots[PANEL_ROWS];
} PanelState;

void initialiseOutputs(void);
PanelState initialise_display_map(void);
void resetPanel(void);
void advanceCol(void);
void advanceRow(void);
void goToDot(uint8_t, uint8_t);
void selectPanel1(void);
void selectPanel2(void);
void selectPanel3(void);
void selectPanel4(void);
void prepDotState(bool);
void powerPulse(void);
void test(void);
void clear_row(void);
void set_row(void);
void clearPanel(void);
void setPanel(void);
void clearDisplay(void);

#endif