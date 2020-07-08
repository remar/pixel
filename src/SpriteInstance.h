#ifndef SPRITE_INSTANCE_H
#define SPRITE_INSTANCE_H

#include "Point.h"

class SpriteInstance {
public:
  SpriteInstance(Image* spriteDefinition) : spriteDefinition(spriteDefinition) {
    totalFrames = getWidth()/10;
    totalAnimations = spriteDefinition->height/10;
  }
  void setPosition(int x, int y);
  void setAnimation(int animation);
  void animate(float deltaTime);
  uint32_t *getPixels();
  Point getPosition() {return Point {x, y};}
  int getWidth() {return spriteDefinition->width;}

private:
  Image* spriteDefinition;
  int x = 0;
  int y = 0;
  int animation = 0;
  int animationOffset = 0;
  int totalAnimations = 0;
  int frame = 0;
  int totalFrames = 0;
  float accumulatedFrameTime = 0;
  float frameDuration = 0.10f;
};

#endif
