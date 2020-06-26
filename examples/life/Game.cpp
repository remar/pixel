#include "Game.h"

#include "GameOfLife.cpp"

void
Game::init() {
  // Do one-time setup here
  screen.hideCursor();

  gameOfLife.randomizeField();
}

void
Game::update(float deltaTime) {
  // Do game stuff here; read input and draw graphics

  if(input.quit() || input.pressed(SDLK_ESCAPE)) {
    shouldQuit = true;
  }

  if(input.pressed(SDLK_f)) {
    screen.toggleFullscreen();
  }

  if(input.pressed(SDLK_SPACE)) {
    gameOfLife.randomizeField();
  }

  if(input.pressed(SDLK_p)) {
    gameOfLife.togglePause();
  }

  gameOfLife.update(screen);
}
