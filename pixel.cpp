#include <SDL2/SDL.h>

#include "Screen.cpp"
#include "Input.cpp"
#include "Game.cpp"

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);

  Screen screen;
  Input input;
  Game game(screen, input);

  game.init();

  while(!game.quit()) {
    input.update();

    game.update();

    screen.redraw();

    SDL_Delay(0);
  }
}
