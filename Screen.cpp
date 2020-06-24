#include "Screen.h"
#include <string>
#include <iostream>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Screen::Screen() {
  window = SDL_CreateWindow("pixel",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    width, height,
			    SDL_WINDOW_SHOWN);
  assert(window != 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  assert(renderer != 0);
  SDL_RenderSetLogicalSize(renderer, width, height);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			      SDL_TEXTUREACCESS_STREAMING,
			      width, height);
  assert(texture != 0);

  clearTilemap();
  clear();
}

Screen::~Screen() {
  deleteTileset();

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void
Screen::clear() {
  memset(screen, 0, width*height*4);
}

void
Screen::redraw() {
  SDL_UpdateTexture(texture, 0, screen, width * 4);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, 0, 0);
  SDL_RenderPresent(renderer);
}

void
Screen::loadTileset(std::string filename) {
  int w, h, n;
  unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, 0);
  assert(n == 4);
  if(data) {
    deleteTileset();
    createTileset(w, h);

    uint8_t *pixels = data;
    for(int i = 0;i < w*h;i++) {
      uint8_t r = *pixels;
      pixels++;
      uint8_t g = *pixels;
      pixels++;
      uint8_t b = *pixels;
      pixels++;
      uint8_t a = *pixels;
      pixels++;

      tileset[i] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
    }

    stbi_image_free(data);
  }
}

void
Screen::drawTiles() {
  for(int i = 0;i < tilesPerColumn*tilesPerRow;i++) {
    if(tilemap[i] != nullptr) {
      // TODO: too naive, maybe?
      drawTile(tilemap[i], i % tilesPerRow, i / tilesPerRow);
    }
  }
}

void
Screen::setTile(int x, int y, int index) {
  assert(x >= 0);
  assert(x < tilesPerRow);
  assert(y >= 0);
  assert(y < tilesPerColumn);
  assert(index >= 0);
  assert(index < tilesInTileset);
  assert(tileset != nullptr);

  int tilesPerRowInTileset = tilesetWidth / tileWidth;
  int offsetX = index % tilesPerRowInTileset;
  int offsetY = index / tilesPerRowInTileset;
  int offset = offsetX * tileWidth + offsetY * tileWidth * tileHeight * tilesPerRowInTileset;

  tilemap[x + y*tilesPerRow] = tileset + offset;
}

void
Screen::toggleFullscreen() {
  fullscreen = !fullscreen;
  SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void
Screen::drawTile(uint32_t *tile, int x, int y) {
  uint32_t *s = &screen[x * tileWidth + y * tileHeight * width];
  for(int tileY = 0;tileY < tileHeight;tileY++) {
    memcpy(s, tile, tileWidth*sizeof(uint32_t));
    s += width;
    tile += tilesetWidth;
  }
}

void
Screen::createTileset(int width, int height) {
  tileset = new uint32_t[width*height];
  tilesetWidth = width;
  tilesInTileset = (width*height)/(tileWidth*tileHeight);

  clearTilemap();
}

void
Screen::deleteTileset() {
  if(tileset) {
    delete[] tileset;
  }
}

void
Screen::clearTilemap() {
  memset(tilemap, 0, tilesPerRow*tilesPerColumn*sizeof(uint32_t *));
}
