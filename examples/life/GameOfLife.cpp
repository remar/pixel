#include "GameOfLife.h"

void
GameOfLife::update(Screen &screen) {
  if(paused) {
    return;
  }

  for(int y = 0;y < Screen::height;y++) {
    for(int x = 0;x < Screen::width;x++) {
      updateCell(x, y);
    }
  }

  int temp = current;
  current = next;
  next = temp;

  for(int y = 0;y < Screen::height;y++) {
    for(int x = 0;x < Screen::width;x++) {
      screen.setPixel(x, y, field[current][x][y] ? 0xffffff : 0);
    }
  }
}


void
GameOfLife::randomizeField() {
  for(int y = 0;y < Screen::height;y++) {
    for(int x = 0;x < Screen::width;x++) {
      field[0][x][y] = rand() % 2;
      field[1][x][y] = rand() % 2;
    }
  }
}

void
GameOfLife::togglePause() {
  paused = !paused;
}

void
GameOfLife::updateCell(int x, int y) {
  int neighbours = countNeighbours(x, y);
  if(isAlive(x, y)) {
    field[next][x][y] = (neighbours == 2 || neighbours == 3) ? 1 : 0;
  } else {
    field[next][x][y] = neighbours == 3 ? 1 : 0;
  }
}

int
GameOfLife::countNeighbours(int x, int y) {
  int offX[] = {-1,  0,  1, -1, 1, -1, 0, 1};
  int offY[] = {-1, -1, -1,  0, 0,  1, 1, 1};
  int count = 0;
  for(int i = 0;i < 8;i++) {
    count += isAlive(x + offX[i], y + offY[i]) ? 1 : 0;
  }
  return count;
}

bool
GameOfLife::isAlive(int x, int y) {
  if(x < 0 || x >= Screen::width || y < 0 || y >= Screen::height) {
    return false;
  }

  return field[current][x][y] > 0;
}
