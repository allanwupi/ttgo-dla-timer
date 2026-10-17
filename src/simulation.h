#ifndef SIMULATION_H
#define SIMULATION_H
#include <TFT_eSPI.h>

#define MAX_PARTICLE_COUNT 3000
#define NUM_WALK_DIRECTIONS 8 // Moore
#define MAX_SPAWN_RADIUS 85
#define COLS 320
#define ROWS 170
#define CENTRE_X 160
#define CENTRE_Y 85
#define NUM_COLOURS 10

typedef enum {
  OUT_OF_BOUNDS = -3,
  LIVE = -2,
  GARBAGE = -1,
  EMPTY = 0,
  DEAD = 1,
  // positive values are ages which get mapped to colours
} State;

typedef struct {
  int x;
  int y;
  int age;
} Walker;

/*int VECTOR_X[NUM_WALK_DIRECTIONS] = {1, 0, -1, 0};
int VECTOR_Y[NUM_WALK_DIRECTIONS] = {0, 1, 0, -1};*/

// DEFINE COLOURS
extern uint32_t TEXT_COLOUR;
extern uint32_t LIVE_COLOUR;
extern uint32_t BACKGROUND_COLOUR;
extern uint32_t HUE[NUM_COLOURS];
extern uint32_t SHADE[NUM_COLOURS];
extern int STAGE[NUM_COLOURS];

extern uint32_t *COLOURSCHEME;
extern bool ENABLE_COLOUR;
extern bool SHOW_PARTICLES;

extern int OLD_PARTICLE_COUNT;
extern int GLOBAL_PARTICLE_COUNT;
extern int VECTOR_X[NUM_WALK_DIRECTIONS];
extern int VECTOR_Y[NUM_WALK_DIRECTIONS];

extern TFT_eSPI tft;

// LOGIC FUNCTIONS
void seed(int grid[][COLS], int select);
void spawn(int grid[][COLS], Walker *ptr, int radius);
bool outOfBounds(int grid[][COLS], int x, int y);
int walk(int grid[][COLS], Walker *ptr);
int stick(int grid[][COLS], Walker *ptr);

// DISTANCE MEASURES
int manhattan(int grid[][COLS]);
int euclidean(int grid[][COLS]);

// VISUALISER FUNCTIONS
void toggleColours(TFT_eSPI *tft);
void toggleParticles(TFT_eSPI *tft);
uint32_t colourMap(int state);
void drawGrid(int grid[][COLS], TFT_eSPI *tft);
void simulate(int grid[][COLS], TFT_eSPI *tft);

#endif