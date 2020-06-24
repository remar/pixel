#ifndef SCREEN_H
#define SCREEN_H

#include <cassert>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>

class Screen {
public:
  static const int width = 320;
  static const int height = 180;
  static const int tileWidth = 10;
  static const int tileHeight = 10;
  static const int tilesPerRow = width/tileWidth;
  static const int tilesPerColumn = height/tileHeight;

  Screen();
  ~Screen();
  void clear();
  void setup();
  void redraw();

  void loadTileset(std::string filename);
  void drawTiles();
  void setTile(int x, int y, int index);

  void toggleFullscreen();
  void showCursor() {SDL_ShowCursor(SDL_ENABLE);}
  void hideCursor() {SDL_ShowCursor(SDL_DISABLE);}

  void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    setPixel(x, y, (red << 16) | (green << 8) | blue);
  }

  void setPixel(int x, int y, uint32_t color) {
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);

    screen[x + y * width] = color;
  }

  uint32_t getPixel(int x, int y) {
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);

    return screen[x + y * width];
  }

private:
  void drawTile(uint32_t *tile, int x, int y);
  void createTileset(int width, int height);
  void deleteTileset();
  void clearTilemap();

  uint32_t screen[width*height];

  uint32_t *tilemap[tilesPerRow*tilesPerColumn];
  uint32_t *tileset = nullptr;
  int tilesetWidth;
  int tilesInTileset;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  bool fullscreen = false;
};

#endif
