#include <TFT_eSPI.h>
#include "simulation.h"

// Treat these colours as constants
uint32_t RGB_TEXT_COLOUR = TFT_WHITE;
uint32_t BW_TEXT_COLOUR = TFT_GOLD;
uint32_t RGB_LIVE_COLOUR = TFT_DARKGREY;
uint32_t BW_LIVE_COLOUR = TFT_DARKGREY;
uint32_t BACKGROUND_COLOUR = TFT_BLACK;

uint32_t TEXT_COLOUR = RGB_TEXT_COLOUR;
uint32_t LIVE_COLOUR = RGB_LIVE_COLOUR;
uint32_t TEMP = BACKGROUND_COLOUR; // holds live colour/background

bool ENABLE_COLOUR = true;
bool SHOW_PARTICLES = true;

uint32_t HUE[NUM_COLOURS] = {
  TFT_WHITE,
  TFT_PINK,
  TFT_MAGENTA,
  TFT_RED,
  TFT_ORANGE,
  TFT_YELLOW,
  TFT_GREEN,
  TFT_CYAN,
  TFT_BLUE,
  TFT_PURPLE,
};

uint32_t SHADE[] = {
  0xFFFF, // 31
  0xEF7D, // 29
  0xDEFB, // 27
  0xC638, // 24
  0xAD75, // 21
  0x94B2, // 18
  0x8430, // 16
  0x73AE, // 14
  0x632C, // 12
  0x528A, // 10
};

int STAGE[NUM_COLOURS] = {
  100,
  200,
  300,
  450,
  600,
  900,
  1200,
  1800,
  2400,
  3000,
};

uint32_t *COLOURSCHEME = HUE;

void toggleColours(TFT_eSPI *tft) {
  ENABLE_COLOUR = !ENABLE_COLOUR;
  if (ENABLE_COLOUR) {
    TEXT_COLOUR = RGB_TEXT_COLOUR;
    if (SHOW_PARTICLES) LIVE_COLOUR = RGB_LIVE_COLOUR;
    else TEMP = RGB_LIVE_COLOUR;
    COLOURSCHEME = HUE;
  } else {
    TEXT_COLOUR = BW_TEXT_COLOUR;
    if (SHOW_PARTICLES) LIVE_COLOUR = BW_LIVE_COLOUR;
    else TEMP = BW_LIVE_COLOUR;
    COLOURSCHEME = SHADE;
  }
  tft->setTextColor(TEXT_COLOUR, BACKGROUND_COLOUR);
}

void toggleParticles(TFT_eSPI *tft) {
  SHOW_PARTICLES = !SHOW_PARTICLES;
  if (SHOW_PARTICLES) {
    LIVE_COLOUR = TEMP;
    TEMP = BACKGROUND_COLOUR;
  } else {
    TEMP = LIVE_COLOUR;
    LIVE_COLOUR = BACKGROUND_COLOUR;
  }
}

uint32_t colourMap(int state) {
  switch (state) {
    case LIVE:
      return LIVE_COLOUR;
    case GARBAGE:
    case EMPTY:
      return BACKGROUND_COLOUR;
    default: // particle age
      for (int i = 0; i < NUM_COLOURS; i++)
        if (state < STAGE[i]) return COLOURSCHEME[i];
      return COLOURSCHEME[NUM_COLOURS-1];
  }
}

void drawGrid(int grid[][COLS], TFT_eSPI *tft) {
  if (grid == NULL) return;
  int cell_colour;
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      if (grid[y][x] == OUT_OF_BOUNDS || grid[y][x] == EMPTY) continue;
      cell_colour = colourMap(grid[y][x]);
      tft->drawPixel(x, y, cell_colour);
      if (grid[y][x] == LIVE) {
        grid[y][x] = GARBAGE;
        continue;
      } 
      if (grid[y][x] == GARBAGE) {
        grid[y][x] = EMPTY;
      }
    }
  }
}