#include <SDL2/SDL.h>

#include "Screen.cpp"
#include "Input.cpp"
#include "Game.cpp"

#include <algorithm>

int lastFrameTime = SDL_GetTicks();

float getDeltaTime() {
  int delta = std::max(SDL_GetTicks() - lastFrameTime, (Uint32)1);
  lastFrameTime = SDL_GetTicks();
  return (float)delta/1000.0f;
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);

  Screen screen;
  Input input;
  Game game(screen, input);

  game.init();

  while(!game.quit()) {
    input.update();

    game.update(getDeltaTime());

    screen.redraw();

    SDL_Delay(0);
  }
}
