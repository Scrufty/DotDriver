
#ifndef HUGEFONT_H
#define HUGEFONT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t width;    
    const uint16_t *cols;   // pointer to list of columns
} Glyph;

extern const Glyph font[];  // no set size

#endif