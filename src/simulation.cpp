#include <TFT_eSPI.h>
#include "simulation.h"
#include "math.h"

int OLD_PARTICLE_COUNT = 0;
int GLOBAL_PARTICLE_COUNT = 0;
char buffer[10];

int VECTOR_X[NUM_WALK_DIRECTIONS] = {1, 1, 0, -1, -1, -1, 0, 1};
int VECTOR_Y[NUM_WALK_DIRECTIONS] = {0, 1, 1, 1, 0, -1, -1, -1};

void seed(int grid[][COLS], int select) {
  switch (select) {
    case 2: // Circle
      for (int o = 0; o < 360; o += 2) {
        float angle = o * M_PI / 180.0;
        int x = round(80 * cos(angle) + CENTRE_X);
        int y = round(80 * sin(angle) + CENTRE_Y);
        grid[y][x] = DEAD;
      }
      break;
    case 1: // Bottom line
      for (int x = 0; x < COLS; x++) grid[ROWS-1][x] = DEAD;
      break;
    case 0: // Centre
    default: //
      grid[CENTRE_Y][CENTRE_X] = DEAD;
  }
}

bool outOfBounds(int grid[][COLS], int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return true;
  if (grid[y][x] == OUT_OF_BOUNDS) return true;
  else return false;
}

void spawn(int grid[][COLS], Walker *ptr, int radius) {
  if (ptr == NULL) return;
  int x = 0, y = 0;
  if (radius < MAX_SPAWN_RADIUS) {
    float angle = (float)random(0, 360) * M_PI / 180.0;
    x = radius * cos(angle) + CENTRE_X;
    y = radius * sin(angle) + CENTRE_Y;
  } else {
    x = random(0,COLS+1);
    y = random(0,ROWS+1);
  }
  ptr->x = x;
  ptr->y = y;
  ptr->age = GLOBAL_PARTICLE_COUNT;
  if (outOfBounds(grid, x, y) || grid[y][x] != EMPTY || abs(x-CENTRE_X)+abs(y-CENTRE_Y) < radius-5) {
    spawn(grid, ptr, radius);
  } else {
    grid[ptr->y][ptr->x] = LIVE;
  }
}

// returns 0 if particle moved successfully within bounds
// returns 1 if particle moved out of bounds
int walk(int grid[][COLS], Walker *ptr) {
  int direction = random(0, NUM_WALK_DIRECTIONS);
  ptr->x += VECTOR_X[direction];
  ptr->y += VECTOR_Y[direction];
  if (outOfBounds(grid, ptr->x, ptr->y)) {
    return 1;
  } else {
    return 0;
  }
}

// returns 1 if non-empty cell detected within Moore neighbourhood
// returns 0 if no non-empty cells are in the neighbourhood
int stick(int grid[][COLS], Walker *ptr) {
  int nx, ny;
  for (int i = 0; i < NUM_WALK_DIRECTIONS; i++) {
    nx = ptr->x + VECTOR_X[i];
    ny = ptr->y + VECTOR_Y[i];
    if (outOfBounds(grid, nx, ny)) {
      continue;
    }
    if (grid[ny][nx] != EMPTY && grid[ny][nx] != LIVE && grid[ny][nx] != GARBAGE) {
      grid[ptr->y][ptr->x] = ptr->age;
      return 1;
    }
  }
  return 0;
}

void simulate(int grid[][COLS], TFT_eSPI *tft) {
  static Walker p = {.x = CENTRE_X, .y = CENTRE_Y};
  static int radius = 3;
  static int margin = 0;
  static int growth_bar = 20;
  static bool respawn = false;
  static bool draw_screen = false;
  static int remaining = MAX_PARTICLE_COUNT;
  if (walk(grid, &p) == 1) {
    respawn = true;
  }
  if (stick(grid, &p) == 1) {
    GLOBAL_PARTICLE_COUNT++;
    remaining--;
    if (radius < MAX_SPAWN_RADIUS && GLOBAL_PARTICLE_COUNT % growth_bar == 0)
      radius = euclidean(grid) + margin;
    respawn = true;
    draw_screen = true;
  }
  if (respawn) {
    spawn(grid, &p, radius);
    respawn = false;
  }
  if (draw_screen) {
    drawGrid(grid, tft);
    draw_screen = false;
    tft->setCursor(0,0);
    if (SHOW_PARTICLES) {
        snprintf(buffer, sizeof(buffer), " %01d:%02d ", remaining/600, remaining/10 % 60);
        tft->drawString(buffer, CENTRE_X+1, TEXT_OFFSET);
    } else {
        tft->drawString("      ", CENTRE_X+1, TEXT_OFFSET);
    }
  }
}

int manhattan(int grid[][COLS]) {
  int max_dist = 0;
  int curr_dist;
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      if (grid[y][x] > 0) {
        curr_dist = abs(x-CENTRE_X) + abs(y-CENTRE_Y);
        if (curr_dist > max_dist) max_dist = curr_dist;
      }
    }
  }
  return max_dist;
}

int euclidean(int grid[][COLS]) {
  int max_dist = 0;
  int curr_dist;
  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
      if (grid[y][x] > 0) {
        curr_dist = (x-CENTRE_X)*(x-CENTRE_X) + (y-CENTRE_Y)*(y-CENTRE_Y);
        if (curr_dist > max_dist) max_dist = curr_dist;
      }
    }
  }
  return round(sqrt(max_dist));
}