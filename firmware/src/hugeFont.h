
#ifndef HUGEFONT_H
#define HUGEFONT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t width;    
    const uint16_t *cols;   // pointer to list of columns
} Glyph;

typedef struct {
    uint8_t width;    
    const uint8_t *cols;   // pointer to list of columns
} MiniGlyph;

extern const Glyph font[];  // no set size

extern const Glyph BoldFont[];  // no set size

extern const MiniGlyph fiveXthreeFont[];  // 5x3

#endif