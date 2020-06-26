#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "Screen.h"

class GameOfLife {
public:
  void update(Screen &screen);
  void randomizeField();
  void togglePause();

private:
  void updateCell(int x, int y);
  int countNeighbours(int x, int y);
  bool isAlive(int x, int y);

  uint32_t field[2][Screen::width][Screen::height];
  int current = 0;
  int next = 1;
  bool paused = false;
};

#endif
