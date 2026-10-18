#include <Arduino.h>
#include <TFT_eSPI.h>
#include "simulation.h"

#define DEFAULT_ROTATION 4
#define RANDOM_SEED_PIN 1
#define DELAY_MILLIS 100

TFT_eSPI tft = TFT_eSPI();
int world[ROWS][COLS] = {0};
int prev;

bool LEFT_BUTTON(void);
bool RIGHT_BUTTON(void);
bool TIMER = true;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(DEFAULT_ROTATION);
  tft.fillScreen(BACKGROUND_COLOUR);
  tft.setTextColor(TEXT_COLOUR, BACKGROUND_COLOUR);
  tft.setTextDatum(TC_DATUM); 
  tft.setTextFont(0);
  tft.setTextSize(5);
  randomSeed(analogRead(RANDOM_SEED_PIN));

  for (int x = 0; x < 30; x++) {
    for (int y = 0; y < 9; y++) {
      world[y][x] = OUT_OF_BOUNDS;
    }
  }
  seed(world, 0);
  prev = millis();
}

void loop() {
  if (LEFT_BUTTON()) toggleColours(&tft);
  if (RIGHT_BUTTON()) toggleParticles(&tft);
  if (GLOBAL_PARTICLE_COUNT >= MAX_PARTICLE_COUNT) {
    toggleColours(&tft);
    drawGrid(world, &tft); // removes last live particle
    tft.setCursor(0,0);
    tft.drawString("       ", CENTRE_X, TEXT_OFFSET); // hides particle count
    delay(999999999);
  } else {
    OLD_PARTICLE_COUNT = GLOBAL_PARTICLE_COUNT;
    simulate(world, &tft);
    if (TIMER && OLD_PARTICLE_COUNT != GLOBAL_PARTICLE_COUNT) {
        delay(DELAY_MILLIS-millis()+prev);
        prev = millis();
    }
  }
}

bool LEFT_BUTTON(void) {
  static bool prevLeft = true;
  static bool currLeft = false;
  currLeft = !digitalRead(0);
  bool leftButtonPressed = (!prevLeft && currLeft);
  prevLeft = currLeft;
  return leftButtonPressed;
}

bool RIGHT_BUTTON(void) {
  static bool prevRight = true;
  static bool currRight = false;
  currRight = !digitalRead(14);
  bool rightButtonPressed = (!prevRight && currRight);
  prevRight = currRight;
  return rightButtonPressed;
}