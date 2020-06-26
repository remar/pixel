#ifndef GAME_H
#define GAME_H

#include "GameOfLife.h"

class Game {
public:
  Game(Screen &screen, Input &input) : screen(screen), input(input) {}
  void init();
  void update(float deltaTime);
  bool quit() {return shouldQuit;}

private:
  Screen &screen;
  Input &input;
  bool shouldQuit = false;
  GameOfLife gameOfLife;
};

#endif
