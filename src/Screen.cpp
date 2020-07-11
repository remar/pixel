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

    uint32_t offset = 0;
    uint32_t screenOffset = 0;
    uint32_t frameWidth = spriteWidth;
    uint32_t frameHeight = spriteHeight;
    int spriteDefWidth = spriteInstance->getWidth();
    Point position = spriteInstance->getPosition();
    if(position.x < -spriteWidth
       || position.x > Screen::width
       || position.y < -spriteHeight
       || position.y > Screen::height) {
      continue;
    }

    if(position.x < 0) {
      screenOffset = offset = -position.x;
      frameWidth = spriteWidth - offset;
    } else if(position.x + spriteWidth > Screen::width) {
      frameWidth = Screen::width - position.x;
    }

    if(position.y < 0) {
      offset += spriteDefWidth * -position.y;
      screenOffset += Screen::width * -position.y;
      frameHeight += position.y;
    } else if(position.y + spriteHeight > Screen::height) {
      frameHeight = Screen::height - position.y;
    }

    uint32_t *pixels = spriteInstance->getPixels() + offset;
    uint32_t *s = &screen[position.x + screenOffset + position.y * width];
    int offsetToNextScreenRow = (width - frameWidth);
    int offsetToNextSpriteRow = (spriteDefWidth - frameWidth);
    for(int spriteY = 0;spriteY < frameHeight;spriteY++) {
      for(int spriteX = 0;spriteX < frameWidth;spriteX++) {
	if(*pixels & 0xff000000) {
	  *s = *pixels;
	}
	s++;
	pixels++;
      }
      s += offsetToNextScreenRow;
      pixels += offsetToNextSpriteRow;
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
