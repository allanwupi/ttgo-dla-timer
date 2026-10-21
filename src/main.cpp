#include <Arduino.h>
#include <TFT_eSPI.h>
#include "simulation.h"

// Set font size 6 for < 10 minutes
#define TIMER_FONT_SIZE 6
#define DEFAULT_ROTATION 2
#define RANDOM_SEED_PIN 1
#define LEFT_PIN 0
#define RIGHT_PIN 14

TFT_eSPI tft = TFT_eSPI();
int world[ROWS][COLS] = {0};
int screen_orientation = DEFAULT_ROTATION;
unsigned long prev;
unsigned long next;
bool left_button_press(void);
bool right_button_press(void);

void setup() {
    //Serial.begin(115200);
    tft.init();
    tft.setRotation(screen_orientation);
    tft.fillScreen(BACKGROUND_COLOUR);
    tft.setTextColor(TEXT_COLOUR, BACKGROUND_COLOUR);
    tft.setTextDatum(TC_DATUM); 
    tft.setTextFont(0);
    tft.setTextSize(TIMER_FONT_SIZE);
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
        scale_time();
    }
    if (right_button_press()) {
        state = state+1;
        if (state == 5) {
            if (screen_orientation == 2) {
                screen_orientation = 4;
            } else {
                screen_orientation = 2;
            }
            tft.setRotation(screen_orientation);
            tft.fillScreen(BACKGROUND_COLOUR);
            state = 0;
        } else {
            toggleParticles(&tft);
            if (state % 2 == 0) {
                toggleColours(&tft);
            }
        }
    }
    unsigned long now = millis();
    if (curr_remaining <= 0) {
        delay(INT_MAX);
    } else if (now >= next) {
        prev_remaining = curr_remaining;
        simulate(world, &tft);
        if (prev_remaining != curr_remaining) {
            next += delay_millis;
            // catch up if we're behind
            if ((long)(now - next) > 0) {
                next = now;
            }
            prev = millis();
        }
    } else {
        delay(1);
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