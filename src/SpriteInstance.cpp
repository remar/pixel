#include "SpriteInstance.h"

void
SpriteInstance::setPosition(int x, int y) {
  this->x = x;
  this->y = y;
}

void
SpriteInstance::setAnimation(int animation) {
  assert(animation >= 0);
  assert(animation < totalAnimations);

  this->animation = animation;
  frame = 0;
  accumulatedFrameTime = 0;
  animationOffset = animation * getWidth() * 10;
}

void
SpriteInstance::animate(float deltaTime) {
  accumulatedFrameTime += deltaTime;
  if(accumulatedFrameTime > frameDuration) {
    accumulatedFrameTime -= frameDuration;
    frame = (frame + 1) % totalFrames;
  }
}

inline uint32_t *
SpriteInstance::getPixels() {
  return spriteDefinition->pixels + frame * 10 + animationOffset;;
}
