#include "Screen.h"
#include <string>
#include <iostream>

Screen::Screen() : nextSpriteId(1) {
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  window = SDL_CreateWindow("pixel",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    width*3, height*3,
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
  tileset.clear();

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
  tileset.loadPng(filename);
  clearTilemap();
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
  assert(index < tileset.width*tileset.height/(tileWidth*tileHeight));
  assert(!tileset.empty());

  int tilesPerRowInTileset = tileset.width / tileWidth;
  int offsetX = index % tilesPerRowInTileset;
  int offsetY = index / tilesPerRowInTileset;
  int offset = offsetX * tileWidth + offsetY * tileWidth * tileHeight * tilesPerRowInTileset;

  tilemap[x + y*tilesPerRow] = tileset.pixels + offset;
}

void
Screen::loadSprite(std::string filename) {
  spriteDefinitions[filename] = std::shared_ptr<Image>(new Image(filename));
}

int
Screen::createSpriteInstance(std::string spriteName) {
  int spriteId = nextSpriteId;
  nextSpriteId++;
  spriteInstances[spriteId] = std::shared_ptr<SpriteInstance>(new SpriteInstance(spriteDefinitions.at(spriteName).get()));
  return spriteId;
}

void
Screen::drawSprites(float deltaTime) {
  for(auto &spriteInstancePair : spriteInstances) {
    SpriteInstance *spriteInstance = spriteInstancePair.second.get();

    spriteInstance->animate(deltaTime);
    uint32_t *pixels = spriteInstance->getPixels();
    Point position = spriteInstance->getPosition();
    int spriteDefWidth = spriteInstance->getWidth();
    uint32_t *s = &screen[position.x + position.y * width];
    for(int spriteY = 0;spriteY < tileHeight;spriteY++) {
      memcpy(s, pixels, tileWidth*sizeof(uint32_t));
      s += width;
      pixels += spriteDefWidth;
    }
  }
}

void
Screen::setSpritePosition(int spriteId, int x, int y) {
  spriteInstances.at(spriteId)->setPosition(x, y);
}

void
Screen::setSpriteAnimation(int spriteId, int animation) {
  spriteInstances.at(spriteId)->setAnimation(animation);
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
    tile += tileset.width;
  }
}

void
Screen::clearTilemap() {
  memset(tilemap, 0, tilesPerRow*tilesPerColumn*sizeof(uint32_t *));
}
