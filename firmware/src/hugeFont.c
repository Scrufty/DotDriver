#include "hugeFont.h"

// Classic Huge Font

// A
static const uint16_t glyph_A_cols[] = {
    0b0000000000001111,
    0b0000000011110000,
    0b0000011110000000,
    0b0011110010000000,
    0b1110000010000000,
    0b0011110010000000,
    0b0000011110000000,
    0b0000000011110000,
    0b0000000000001111
};

// B
static const uint16_t glyph_B_cols[] = {
    0b1111111111111111,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b0100001100000011,
    0b0011110011000110,
    0b0000000001111100
};

// C
static const uint16_t glyph_C_cols[] = {
    0b0000111111100000,
    0b0011000000011000,
    0b0110000000001110,
    0b1100000000000011,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1100000000000011,
    0b0110000000000110
};

// D
static const uint16_t glyph_D_cols[] = {
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1100000000000011,
    0b0110000000000110,
    0b0011000000001100,
    0b0000111111110000
};

// E
static const uint16_t glyph_E_cols[] = {
    0b1111111111111111,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001
};

// F
static const uint16_t glyph_F_cols[] = {
    0b1111111111111111,
    0b1000000100000000,
    0b1000000100000000,
    0b1000000100000000,
    0b1000000100000000,
    0b1000000100000000,
    0b1000000000000000,
    0b1000000000000000
};

// G
static const uint16_t glyph_G_cols[] = {
    0b0000111111100000,
    0b0011000000011000,
    0b0110000000001110,
    0b1100000000000011,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000010000001,
    0b1000000010000001,
    0b1100000010000111,
    0b0110000001111100
};

// H
static const uint16_t glyph_H_cols[] = {
    0b1111111111111111,
    0b0000000100000000,
    0b0000000100000000,
    0b0000000100000000,
    0b0000000100000000,
    0b0000000100000000,
    0b0000000100000000,
    0b1111111111111111
};

// I
static const uint16_t glyph_I_cols[] = {
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001
};

// J
static const uint16_t glyph_J_cols[] = {
    0b1000000000011100,
    0b1000000000000010,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000010,
    0b1111111111111100,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000
};

// K
static const uint16_t glyph_K_cols[] = {
    0b1111111111111111,
    0b0000000100000000,
    0b0000001110000000,
    0b0000010011000000,
    0b0000100000110000,
    0b0001000000001100,
    0b0010000000000110,
    0b0100000000000011,
    0b1000000000000001
};

// L
static const uint16_t glyph_L_cols[] = {
    0b1111111111111111,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001
};

// M
static const uint16_t glyph_M_cols[] = {
    0b1111111111111111,
    0b0110000000000000,
    0b0011000000000000,
    0b0001100000000000,
    0b0000110000000000,
    0b0000011000000000,
    0b0000110000000000,
    0b0001100000000000,
    0b0011000000000000,
    0b0110000000000000,
    0b1111111111111111
};

// N
static const uint16_t glyph_N_cols[] = {
    0b1111111111111111,
    0b0110000000000000,
    0b0001100000000000,
    0b0000011000000000,
    0b0000000110000000,
    0b0000000001100000,
    0b0000000000011000,
    0b0000000000000110,
    0b1111111111111111
};

// O
static const uint16_t glyph_O_cols[] = {
    0b0000111111110000,
    0b0011000000001100,
    0b0100000000000010,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b0100000000000010,
    0b0011000000001100,
    0b0000111111110000
};

// P
static const uint16_t glyph_P_cols[] = {
    0b1111111111111111,
    0b1000000010000000,
    0b1000000010000000,
    0b1000000010000000,
    0b1000000100000000,
    0b0100000100000000,
    0b0100000100000000,
    0b0011111000000000
};

// Q
static const uint16_t glyph_Q_cols[] = {
    0b0000111111110000,
    0b0011000000001100,
    0b0100000000000010,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000001001,
    0b0100000000000110,
    0b0011000000001100,
    0b0000111111110010,
    0b0000000000000001
};

// R
static const uint16_t glyph_R_cols[] = {
    0b1111111111111111,
    0b1000000010000000,
    0b1000000010000000,
    0b1000000011000000,
    0b1000000101110000,
    0b0100000100001100,
    0b0100000100000011,
    0b0011111000000001
};

// S
static const uint16_t glyph_S_cols[] = {
    0b0011110000000010,
    0b0100001000000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000100000001,
    0b1000000010000001,
    0b1000000010000010,
    0b0100000001111100
};

// T
static const uint16_t glyph_T_cols[] = {
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1111111111111111,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000,
    0b1000000000000000
};

// U
static const uint16_t glyph_U_cols[] = {
    0b1111111111110000,
    0b0000000000001100,
    0b0000000000000010,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000001,
    0b0000000000000010,
    0b0000000000001100,
    0b1111111111110000,
};

// V
static const uint16_t glyph_V_cols[] = {
    0b1111000000000000,
    0b0001111000000000,
    0b0000001111000000,
    0b0000000001111000,
    0b0000000000001110,
    0b0000000000000001,
    0b0000000000001110,
    0b0000000001111000,
    0b0000001111000000,
    0b0001111000000000,
    0b1111000000000000
};

// W
static const uint16_t glyph_W_cols[] = {
    0b1111111000000000,
    0b0000001111111000,
    0b0000000000001111,
    0b0000000000000100,
    0b0000000000011000,
    0b0000000000110000,
    0b0000000000011000,
    0b0000000000000100,
    0b0000000000001111,
    0b0000001111111000,
    0b1111111000000000
};

// X
static const uint16_t glyph_X_cols[] = {
    0b1000000000000001,
    0b0110000000000110,
    0b0001100000011000,
    0b0000011001100000,
    0b0000000110000000,
    0b0000011001100000,
    0b0001100000011000,
    0b0110000000000110,
    0b1000000000000001,
};

// Y
static const uint16_t glyph_Y_cols[] = {
    0b1110000000000000,
    0b0011000000000000,
    0b0001100000000000,
    0b0000110000000000,
    0b0000011111111111,
    0b0000110000000000,
    0b0001100000000000,
    0b0011000000000000,
    0b1110000000000000
};

// Z
static const uint16_t glyph_Z_cols[] = {
    0b1000000000000111,
    0b1000000000011101,
    0b1000000001110001,
    0b1000000111000001,
    0b1000001100000001,
    0b1000011000000001,
    0b1000110000000001,
    0b1011000000000001,
    0b1100000000000001
};

// SPACE
static const uint16_t glyph_SPACE_cols[] = {
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
};

// EXP
static const uint16_t glyph_EXP_cols[] = {
    0b1111111111111001
};

// Font lookup table
const Glyph font[] = {
    ['A'] = { .width = 9, .cols = glyph_A_cols},
    ['B'] = { .width = 9, .cols = glyph_B_cols},
    ['C'] = { .width = 10, .cols = glyph_C_cols},
    ['D'] = { .width = 9, .cols = glyph_D_cols},
    ['E'] = { .width = 8, .cols = glyph_E_cols},
    ['F'] = { .width = 8, .cols = glyph_F_cols},
    ['G'] = { .width = 10, .cols = glyph_G_cols},
    ['H'] = { .width = 8, .cols = glyph_H_cols},
    ['I'] = { .width = 7, .cols = glyph_I_cols},
    ['J'] = { .width = 10, .cols = glyph_J_cols},
    ['K'] = { .width = 9, .cols = glyph_K_cols},
    ['L'] = { .width = 9, .cols = glyph_L_cols},
    ['M'] = { .width = 11, .cols = glyph_M_cols},
    ['N'] = { .width = 9, .cols = glyph_N_cols},
    ['O'] = { .width = 11, .cols = glyph_O_cols},
    ['P'] = { .width = 8, .cols = glyph_P_cols},
    ['Q'] = { .width = 12, .cols = glyph_Q_cols},
    ['R'] = { .width = 8, .cols = glyph_R_cols},
    ['S'] = { .width = 9, .cols = glyph_S_cols},
    ['T'] = { .width = 9, .cols = glyph_T_cols},
    ['U'] = { .width = 10, .cols = glyph_U_cols},
    ['V'] = { .width = 11, .cols = glyph_V_cols},
    ['W'] = { .width = 11, .cols = glyph_W_cols},
    ['X'] = { .width = 9, .cols = glyph_X_cols},
    ['Y'] = { .width = 9, .cols = glyph_Y_cols},
    ['Z'] = { .width = 9, .cols = glyph_Z_cols},
    ['_'] = { .width = 3, .cols = glyph_SPACE_cols},
    ['!'] = { .width = 1, .cols = glyph_EXP_cols},
};


// Bold Huge Font

// A
static const uint16_t BOLDglyph_A_cols[] = {
    0b0000000000001111,
    0b0000000011111100,
    0b0000011111100000,
    0b0011110011000000,
    0b1110000011000000,
    0b0011110011000000,
    0b0000011111100000,
    0b0000000011111100,
    0b0000000000001111
};

// B
static const uint16_t BOLDglyph_B_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100000110000011,
    0b1100000110000011,
    0b1100000110000011,
    0b1100000110000011,
    0b0110011110000111,
    0b0011110011111110,
    0b0000000001111100
};

// C
static const uint16_t BOLDglyph_C_cols[] = {
    0b0000111111100000,
    0b0011111111111000,
    0b0111000000001110,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1110000000000111,
    0b0111000000001110
};

// D
static const uint16_t BOLDglyph_D_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b0111000000000110,
    0b0011111111111100,
    0b0000111111110000
};

// E
static const uint16_t BOLDglyph_E_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100001100000011,
    0b1100001100000011,
    0b1100001100000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011
};

// F
static const uint16_t BOLDglyph_F_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100001100000000,
    0b1100001100000000,
    0b1100001100000000,
    0b1100001100000000,
    0b1100000000000000,
    0b1100000000000000
};

// G
static const uint16_t BOLDglyph_G_cols[] = {
    0b0000111111100000,
    0b0011111111111000,
    0b0111000000001110,
    0b1110000000000111,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000011000011,
    0b1100000011000011,
    0b1110000011100111,
    0b0111000001111100
};

// H
static const uint16_t BOLDglyph_H_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b0000000110000000,
    0b0000000110000000,
    0b0000000110000000,
    0b0000000110000000,
    0b1111111111111111,
    0b1111111111111111
};

// I
static const uint16_t BOLDglyph_I_cols[] = {
    0b1100000000000011,
    0b1100000000000011,
    0b1111111111111111,
    0b1111111111111111,
    0b1100000000000011,
    0b1100000000000011
};

// J
static const uint16_t BOLDglyph_J_cols[] = {
    0b1100000000111100,
    0b1100000000001110,
    0b1100000000000011,
    0b1100000000000011,
    0b1111111111111110,
    0b1111111111111100,
    0b1100000000000000,
    0b1100000000000000,
    0b1100000000000000,
    0b1100000000000000
};

// K
static const uint16_t BOLDglyph_K_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b0000001111000000,
    0b0000011011100000,
    0b0000110000111000,
    0b0001100000001100,
    0b0011000000000110,
    0b0110000000000011,
    0b1100000000000001
};

// L
static const uint16_t BOLDglyph_L_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011
};

// M
static const uint16_t BOLDglyph_M_cols[] = {
    0b1111111111111111,
    0b0111111111111111,
    0b0011000000000000,
    0b0001100000000000,
    0b0000110000000000,
    0b0000011000000000,
    0b0000110000000000,
    0b0001100000000000,
    0b0011000000000000,
    0b0111111111111111,
    0b1111111111111111
};

// N
static const uint16_t BOLDglyph_N_cols[] = {
    0b1111111111111111,
    0b0111111111111111,
    0b0001110000000000,
    0b0000011100000000,
    0b0000000111000000,
    0b0000000001110000,
    0b0000000000011100,
    0b1111111111111111,
    0b1111111111111111
};

// O
static const uint16_t BOLDglyph_O_cols[] = {
    0b0000111111110000,
    0b0011111111111100,
    0b0110000000000110,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b0110000000000110,
    0b0011111111111100,
    0b0000111111110000
};

// P
static const uint16_t BOLDglyph_P_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100000011000000,
    0b1100000011000000,
    0b1100000110000000,
    0b0110000110000000,
    0b0111111100000000,
    0b0011111000000000
};

// Q
static const uint16_t BOLDglyph_Q_cols[] = {
    0b0000111111110000,
    0b0011111111111100,
    0b0110000000000110,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000011011,
    0b0110000000001110,
    0b0011111111111100,
    0b0000111111110011,
    0b0000000000000001
};

// R
static const uint16_t BOLDglyph_R_cols[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1100000011000000,
    0b1100000011000000,
    0b1100000011110000,
    0b0110000110011100,
    0b0111111100000111,
    0b0011111000000001
};

// S
static const uint16_t BOLDglyph_S_cols[] = {
    0b0011110000000110,
    0b0111111100000011,
    0b1100000110000011,
    0b1100000110000011,
    0b1100000110000011,
    0b1100000110000011,
    0b1100000011000011,
    0b1100000011111110,
    0b0110000001111100
};

// T
static const uint16_t BOLDglyph_T_cols[] = {
    0b1100000000000000,
    0b1100000000000000,
    0b1100000000000000,
    0b1111111111111111,
    0b1111111111111111,
    0b1100000000000000,
    0b1100000000000000,
    0b1100000000000000
};

// U
static const uint16_t BOLDglyph_U_cols[] = {
    0b1111111111110000,
    0b1111111111111110,
    0b0000000000000110,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000011,
    0b0000000000000110,
    0b1111111111111110,
    0b1111111111110000,
};

// V
static const uint16_t BOLDglyph_V_cols[] = {
    0b1111100000000000,
    0b0001111100000000,
    0b0000001111100000,
    0b0000000001111100,
    0b0000000000001110,
    0b0000000000000011,
    0b0000000000001110,
    0b0000000001111100,
    0b0000001111100000,
    0b0001111100000000,
    0b1111100000000000
};

// W
static const uint16_t BOLDglyph_W_cols[] = {
    0b1111111000000000,
    0b0000011111111000,
    0b0000000000011111,
    0b0000000000000110,
    0b0000000000011100,
    0b0000000001110000,
    0b0000000000011100,
    0b0000000000000110,
    0b0000000000011111,
    0b0000011111111000,
    0b1111111000000000
};

// X
static const uint16_t BOLDglyph_X_cols[] = {
    0b1000000000000001,
    0b1110000000000111,
    0b0011100000011100,
    0b0000111001110000,
    0b0000001111000000,
    0b0000111001110000,
    0b0011100000011100,
    0b1110000000000111,
    0b1000000000000001,
};

// Y
static const uint16_t BOLDglyph_Y_cols[] = {
    0b1111000000000000,
    0b0011100000000000,
    0b0001110000000000,
    0b0000111000000000,
    0b0000011111111111,
    0b0000011111111111,
    0b0000111000000000,
    0b0001110000000000,
    0b0011100000000000,
    0b1111000000000000
};

// Z
static const uint16_t BOLDglyph_Z_cols[] = {
    0b1100000000001111,
    0b1100000000111111,
    0b1100000001110011,
    0b1100000111000011,
    0b1100001110000011,
    0b1100011100000011,
    0b1100110000000011,
    0b1111100000000011,
    0b1100000000000011
};

// SPACE
static const uint16_t BOLDglyph_SPACE_cols[] = {
    0b0000000000000000,
    0b0000000000000000
};

// EXP
static const uint16_t BOLDglyph_EXP_cols[] = {
    0b1111111111111001,
    0b1111111111111001
};

// &
static const uint16_t BOLDglyph_AND_cols[] = {
    0b0000000000011100,
    0b0000000001111110,
    0b0011000011000011,
    0b0100110110000001,
    0b1000001100000001,
    0b1000001100000001,
    0b1000011111000001,
    0b1100100000110100,
    0b0111000000001110,
    0b0000000000010001,
    0b0000000000100000,

};

// Verse SPACE (for when " " character appears in string to roll)
static const uint16_t BOLDglyph_verseSPACE_cols[] = {
    0b0000000000000000,
    0b0000000000000000
};

// Bold Font lookup table
const Glyph BoldFont[] = {
    ['A'] = { .width = 9, .cols = BOLDglyph_A_cols},
    ['B'] = { .width = 9, .cols = BOLDglyph_B_cols},
    ['C'] = { .width = 10, .cols = BOLDglyph_C_cols},
    ['D'] = { .width = 9, .cols = BOLDglyph_D_cols},
    ['E'] = { .width = 8, .cols = BOLDglyph_E_cols},
    ['F'] = { .width = 8, .cols = BOLDglyph_F_cols},
    ['G'] = { .width = 10, .cols = BOLDglyph_G_cols},
    ['H'] = { .width = 8, .cols = BOLDglyph_H_cols},
    ['I'] = { .width = 6, .cols = BOLDglyph_I_cols},
    ['J'] = { .width = 10, .cols = BOLDglyph_J_cols},
    ['K'] = { .width = 9, .cols = BOLDglyph_K_cols},
    ['L'] = { .width = 9, .cols = BOLDglyph_L_cols},
    ['M'] = { .width = 11, .cols = BOLDglyph_M_cols},
    ['N'] = { .width = 9, .cols = BOLDglyph_N_cols},
    ['O'] = { .width = 11, .cols = BOLDglyph_O_cols},
    ['P'] = { .width = 8, .cols = BOLDglyph_P_cols},
    ['Q'] = { .width = 12, .cols = BOLDglyph_Q_cols},
    ['R'] = { .width = 8, .cols = BOLDglyph_R_cols},
    ['S'] = { .width = 9, .cols = BOLDglyph_S_cols},
    ['T'] = { .width = 8, .cols = BOLDglyph_T_cols},
    ['U'] = { .width = 10, .cols = BOLDglyph_U_cols},
    ['V'] = { .width = 11, .cols = BOLDglyph_V_cols},
    ['W'] = { .width = 11, .cols = BOLDglyph_W_cols},
    ['X'] = { .width = 9, .cols = BOLDglyph_X_cols},
    ['Y'] = { .width = 10, .cols = BOLDglyph_Y_cols},
    ['Z'] = { .width = 9, .cols = BOLDglyph_Z_cols},
    ['_'] = { .width = 2, .cols = BOLDglyph_SPACE_cols},
    ['!'] = { .width = 2, .cols = BOLDglyph_EXP_cols},
    ['&'] = { .width = 11, .cols = BOLDglyph_AND_cols},
    [' '] = { .width = 2, .cols = BOLDglyph_verseSPACE_cols},
};

// 5x3 Font

// 1
static const uint8_t fiveXthreeglyph_1_cols[] = {
    0b00010001,
    0b00011111,
    0b00010000
};

// 2
static const uint8_t fiveXthreeglyph_2_cols[] = {
    0b00011101,
    0b00010101,
    0b00010111
};

// 3
static const uint8_t fiveXthreeglyph_3_cols[] = {
    0b00010101,
    0b00010101,
    0b00011111
};

// 4
static const uint8_t fiveXthreeglyph_4_cols[] = {
    0b00000111,
    0b00000100,
    0b00011111
};

// 5
static const uint8_t fiveXthreeglyph_5_cols[] = {
    0b00010111,
    0b00010101,
    0b00011101
};

// 6
static const uint8_t fiveXthreeglyph_6_cols[] = {
    0b00011111,
    0b00010101,
    0b00011101
};

// 7
static const uint8_t fiveXthreeglyph_7_cols[] = {
    0b00000001,
    0b00000001,
    0b00011111
};

// 8
static const uint8_t fiveXthreeglyph_8_cols[] = {
    0b00011111,
    0b00010101,
    0b00011111
};

// 9
static const uint8_t fiveXthreeglyph_9_cols[] = {
    0b00010111,
    0b00010101,
    0b00011111
};

// 0
static const uint8_t fiveXthreeglyph_0_cols[] = {
    0b00011111,
    0b00010001,
    0b00011111
};

// :
static const uint8_t fiveXthreeglyph_colon_cols[] = {
    0b00001010
};

// A
static const uint8_t fiveXthreeglyph_A_cols[] = {
    0b00011110,
    0b00000101,
    0b00011110
};

// B
static const uint8_t fiveXthreeglyph_B_cols[] = {
    0b00011111,
    0b00010101,
    0b00001010
};

// C
static const uint8_t fiveXthreeglyph_C_cols[] = {
    0b00001110,
    0b00010001,
    0b00010001
};

// D
static const uint8_t fiveXthreeglyph_D_cols[] = {
    0b00011111,
    0b00010001,
    0b00001110
};

// E
static const uint8_t fiveXthreeglyph_E_cols[] = {
    0b00011111,
    0b00010101,
    0b00010001
};

// F
static const uint8_t fiveXthreeglyph_F_cols[] = {
    0b00011111,
    0b00000101,
    0b00000001
};

// G
static const uint8_t fiveXthreeglyph_G_cols[] = {
    0b00001110,
    0b00010001,
    0b00011010
};

// H
static const uint8_t fiveXthreeglyph_H_cols[] = {
    0b00011111,
    0b00000100,
    0b00011111
};

// I
static const uint8_t fiveXthreeglyph_I_cols[] = {
    0b00010001,
    0b00011111,
    0b00010001
};

// J
static const uint8_t fiveXthreeglyph_J_cols[] = {
    0b00010001,
    0b00011111,
    0b00000001
};

// K
static const uint8_t fiveXthreeglyph_K_cols[] = {
    0b00011111,
    0b00000110,
    0b00011001
};

// L
static const uint8_t fiveXthreeglyph_L_cols[] = {
    0b00011111,
    0b00010000,
    0b00010000
};

// M
static const uint8_t fiveXthreeglyph_M_cols[] = {
    0b00011111,
    0b00000010,
    0b00000100,
    0b00000010,
    0b00011111
};

// N
static const uint8_t fiveXthreeglyph_N_cols[] = {
    0b00011111,
    0b00000010,
    0b00000100,
    0b00011111
};

// O
static const uint8_t fiveXthreeglyph_O_cols[] = {
    0b00001110,
    0b00010001,
    0b00001110
};

// P
static const uint8_t fiveXthreeglyph_P_cols[] = {
    0b00011111,
    0b00000101,
    0b00000010
};

// Q
static const uint8_t fiveXthreeglyph_Q_cols[] = {
    0b00001110,
    0b00010101,
    0b00011110
};

// R
static const uint8_t fiveXthreeglyph_R_cols[] = {    
    0b00011111,
    0b00000101,
    0b00011010
};

// S
static const uint8_t fiveXthreeglyph_S_cols[] = {
    0b00010111,
    0b00010101,
    0b00011101
};

// T
static const uint8_t fiveXthreeglyph_T_cols[] = {
    0b00000001,
    0b00011111,
    0b00000001
};

// U
static const uint8_t fiveXthreeglyph_U_cols[] = {
    0b00011111,
    0b00010000,
    0b00011111
};

// V
static const uint8_t fiveXthreeglyph_V_cols[] = {
    0b00001111,
    0b00010000,
    0b00001111
};

// W
static const uint8_t fiveXthreeglyph_W_cols[] = {
    0b00001111,
    0b00001100,
    0b00000110,
    0b000011100,
    0b00001111
};

// X
static const uint8_t fiveXthreeglyph_X_cols[] = {
    0b00011011,
    0b00000100,
    0b00011011
};

// Y
static const uint8_t fiveXthreeglyph_Y_cols[] = {
    0b00000011,
    0b00011100,
    0b00000011
};

// Z
static const uint8_t fiveXthreeglyph_Z_cols[] = {
    0b00011001,
    0b00010101,
    0b00010011
};

// -
static const uint8_t fiveXthreeglyph_dash_cols[] = {
    0b00000100,
    0b00000100
};

// .
static const uint8_t fiveXthreeglyph_dot_cols[] = {
    0b00000001
};

// "
static const uint8_t fiveXthreeglyph_quote_cols[] = {
    0b11000000,
    0b00000000,
    0b11000000
};

// ,
static const uint8_t fiveXthreeglyph_coma_cols[] = {
    0b00000010,
    0b00000001
};


// 5x3 Font lookup table
const MiniGlyph fiveXthreeFont[] = {
    ['1'] = { .width = 3, .cols = fiveXthreeglyph_1_cols},
    ['2'] = { .width = 3, .cols = fiveXthreeglyph_2_cols},
    ['3'] = { .width = 3, .cols = fiveXthreeglyph_3_cols},
    ['4'] = { .width = 3, .cols = fiveXthreeglyph_4_cols},
    ['5'] = { .width = 3, .cols = fiveXthreeglyph_5_cols},
    ['6'] = { .width = 3, .cols = fiveXthreeglyph_6_cols},
    ['7'] = { .width = 3, .cols = fiveXthreeglyph_7_cols},
    ['8'] = { .width = 3, .cols = fiveXthreeglyph_8_cols},
    ['9'] = { .width = 3, .cols = fiveXthreeglyph_9_cols},
    ['0'] = { .width = 3, .cols = fiveXthreeglyph_0_cols},
    [':'] = { .width = 1, .cols = fiveXthreeglyph_colon_cols},
    ['A'] = { .width = 3, .cols = fiveXthreeglyph_A_cols},
    ['B'] = { .width = 3, .cols = fiveXthreeglyph_B_cols},
    ['C'] = { .width = 3, .cols = fiveXthreeglyph_C_cols},
    ['D'] = { .width = 3, .cols = fiveXthreeglyph_D_cols},
    ['E'] = { .width = 3, .cols = fiveXthreeglyph_E_cols},
    ['F'] = { .width = 3, .cols = fiveXthreeglyph_F_cols},
    ['G'] = { .width = 3, .cols = fiveXthreeglyph_G_cols},
    ['H'] = { .width = 3, .cols = fiveXthreeglyph_H_cols},
    ['I'] = { .width = 3, .cols = fiveXthreeglyph_I_cols},
    ['J'] = { .width = 3, .cols = fiveXthreeglyph_J_cols},
    ['K'] = { .width = 3, .cols = fiveXthreeglyph_K_cols},
    ['L'] = { .width = 3, .cols = fiveXthreeglyph_L_cols},
    ['M'] = { .width = 5, .cols = fiveXthreeglyph_M_cols},
    ['N'] = { .width = 4, .cols = fiveXthreeglyph_N_cols},
    ['O'] = { .width = 3, .cols = fiveXthreeglyph_O_cols},
    ['P'] = { .width = 3, .cols = fiveXthreeglyph_P_cols},
    ['Q'] = { .width = 3, .cols = fiveXthreeglyph_Q_cols},
    ['R'] = { .width = 3, .cols = fiveXthreeglyph_R_cols},
    ['S'] = { .width = 3, .cols = fiveXthreeglyph_S_cols},
    ['T'] = { .width = 3, .cols = fiveXthreeglyph_T_cols},
    ['U'] = { .width = 3, .cols = fiveXthreeglyph_U_cols},
    ['V'] = { .width = 3, .cols = fiveXthreeglyph_V_cols},
    ['W'] = { .width = 5, .cols = fiveXthreeglyph_W_cols},
    ['X'] = { .width = 3, .cols = fiveXthreeglyph_X_cols},
    ['Y'] = { .width = 3, .cols = fiveXthreeglyph_Y_cols},
    ['Z'] = { .width = 3, .cols = fiveXthreeglyph_Z_cols},
    ['-'] = { .width = 2, .cols = fiveXthreeglyph_dash_cols},
    ['.'] = { .width = 1, .cols = fiveXthreeglyph_dot_cols},
    ['"'] = { .width = 3, .cols = fiveXthreeglyph_quote_cols},
    [','] = { .width = 2, .cols = fiveXthreeglyph_coma_cols}
};

// 4x3 Font

// 1
static const uint8_t fourXthreeglyph_1_cols[] = {
    0b00001001,
    0b00001111,
    0b00001000
};

// 2
static const uint8_t fourXthreeglyph_2_cols[] = {
    0b00001101,
    0b00001011,
    0b00001011
};

// 3
static const uint8_t fourXthreeglyph_3_cols[] = {
    0b00001001,
    0b00001011,
    0b00001111
};

// 4
static const uint8_t fourXthreeglyph_4_cols[] = {
    0b00000111,
    0b00000100,
    0b00001111
};

// 5
static const uint8_t fourXthreeglyph_5_cols[] = {
    0b00001011,
    0b00001101,
    0b00001101
};

// 6
static const uint8_t fourXthreeglyph_6_cols[] = {
    0b00001111,
    0b00001010,
    0b00001110
};

// 7
static const uint8_t fourXthreeglyph_7_cols[] = {
    0b00000001,
    0b00000001,
    0b00001111
};

// 8
static const uint8_t fourXthreeglyph_8_cols[] = {
    0b00001100,
    0b00001111,
    0b00000011
};

// 9
static const uint8_t fourXthreeglyph_9_cols[] = {
    0b00000111,
    0b00000101,
    0b00001111
};

// 0
static const uint8_t fourXthreeglyph_0_cols[] = {
    0b00001111,
    0b00001001,
    0b00001111
};

// :
static const uint8_t fourXthreeglyph_colon_cols[] = {
    0b00000000
};

// washing rest pattern (callsign R)
static const uint8_t fourXthreeglyph_washing_rest_cols[] = {
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001
};

// Washing in progress pattern (callsign W)
static const uint8_t fourXthreeglyph_washing_cols[] = {
    0b00000011,
    0b00001100,
    0b00000010,
    0b00001100,
    0b00000011,
    0b00000000,
    0b00001110,
    0b00000101,
    0b00001110,
    0b00000000,
    0b00001010,
    0b00001101,
    0b00000101,
    0b00000000,
    0b00001111,
    0b00000010,
    0b00001111,
    0b00000000,
    0b00001101,
    0b00000000,
    0b00001111,
    0b00000010,
    0b00000100,
    0b00001111,
    0b00000000,
    0b00000110,
    0b00001001,
    0b00001101,
};

// Washing done pattern (callsign D)
static const uint8_t fourXthreeglyph_washing_done_cols[] = {
    0b00001110,
    0b00001010,
    0b00001110,
    0b00000000,
    0b00001110,
    0b00001010,
    0b00001110,
    0b00000000,
    0b00001111,
    0b00000010,
    0b00000100,
    0b00001111,
    0b00000000,
    0b00001111,
    0b00001011,
    0b00001001
};

// washing machine not running (callsign N)
static const uint8_t fourXthreeglyph_washing_not_running_cols[] = {
    0b00001111,
    0b00001000,
    0b00001111,
    0b00000000,
    0b00001111,
    0b00000010,
    0b00000100,
    0b00001111,
    0b00000000,
    0b00001111,
    0b00001000,
    0b00001111,
    0b00000000,
    0b00001010,
    0b00001101,
    0b00000101,
    0b00000000,
    0b00001111,
    0b00001011,
    0b00001001,
    0b00000000,
    0b00001110,
    0b00001010,
    0b00001111
};

// 4x3 Font lookup table
const MiniGlyph fourXthreeFont[] = {
    ['1'] = { .width = 3, .cols = fourXthreeglyph_1_cols},
    ['2'] = { .width = 3, .cols = fourXthreeglyph_2_cols},
    ['3'] = { .width = 3, .cols = fourXthreeglyph_3_cols},
    ['4'] = { .width = 3, .cols = fourXthreeglyph_4_cols},
    ['5'] = { .width = 3, .cols = fourXthreeglyph_5_cols},
    ['6'] = { .width = 3, .cols = fourXthreeglyph_6_cols},
    ['7'] = { .width = 3, .cols = fourXthreeglyph_7_cols},
    ['8'] = { .width = 3, .cols = fourXthreeglyph_8_cols},
    ['9'] = { .width = 3, .cols = fourXthreeglyph_9_cols},
    ['0'] = { .width = 3, .cols = fourXthreeglyph_0_cols},
    ['/'] = { .width = 1, .cols = fourXthreeglyph_colon_cols},
    ['R'] = { .width = 42, .cols = fourXthreeglyph_washing_rest_cols},
    ['W'] = { .width = 28, .cols = fourXthreeglyph_washing_cols},
    ['D'] = { .width = 16, .cols = fourXthreeglyph_washing_done_cols},
    ['N'] = { .width = 24, .cols = fourXthreeglyph_washing_not_running_cols}
};