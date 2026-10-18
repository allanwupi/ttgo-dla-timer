#include <Arduino.h>
#include <TFT_eSPI.h>
#include "simulation.h"

#define DEFAULT_ROTATION 4
#define RANDOM_SEED_PIN 1
#define DELAY_MILLIS 100
#define LEFT_PIN 0
#define RIGHT_PIN 14

TFT_eSPI tft = TFT_eSPI();
int world[ROWS][COLS] = {0};
long prev;
long calculated_delay;
long error = 0;
bool left_button_press(void);
bool right_button_press(void);

void setup() {
    tft.init();
    tft.setRotation(DEFAULT_ROTATION);
    tft.fillScreen(BACKGROUND_COLOUR);
    tft.setTextColor(TEXT_COLOUR, BACKGROUND_COLOUR);
    tft.setTextDatum(TC_DATUM); 
    tft.setTextFont(0);
    tft.setTextSize(6);
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
    static int state = 0;
    if (left_button_press()) {
        add_time(10);
    }
    if (right_button_press()) {
        state = state +1 % 4;
        toggleParticles(&tft);
        if (state % 2 == 0) {
            toggleColours(&tft);
        }
    }
    if (curr_remaining <= 0) {
        delay(INFINITY);
    } else {
        prev_remaining = curr_remaining;
        simulate(world, &tft);
        if (prev_remaining != curr_remaining) {
            calculated_delay = DELAY_MILLIS-error-millis()+prev;
            if (calculated_delay < 0) {
                error += abs(calculated_delay);
            } else {
                error = 0;
                delay(calculated_delay);
            }
            prev = millis();
        }
    }
}

bool left_button_press(void) {
    static bool prevLeft = true;
    static bool currLeft = false;
    currLeft = !digitalRead(LEFT_PIN);
    bool leftButtonPressed = (!prevLeft && currLeft);
    prevLeft = currLeft;
    return leftButtonPressed;
}

bool right_button_press(void) {
    static bool prevRight = true;
    static bool currRight = false;
    currRight = !digitalRead(RIGHT_PIN);
    bool rightButtonPressed = (!prevRight && currRight);
    prevRight = currRight;
    return rightButtonPressed;
}