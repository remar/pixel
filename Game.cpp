#include "Game.h"

void
Game::init() {
  // Do one-time setup here
  screen.hideCursor();
}

void
Game::update(/* TODO: Send in delta time */) {
  // Do game stuff here; read input and draw graphics
  if(input.quit() || input.pressed(SDLK_ESCAPE)) {
    shouldQuit = true;
  }

  if(input.pressed(SDLK_f)) {
    screen.toggleFullscreen();
  }

  auto p = input.mousePosition();

  screen.clear();
  screen.setPixel(p.x, p.y, 255, 0, 0);
}
