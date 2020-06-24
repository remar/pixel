#include "Input.h"

void
Input::update() {
  SDL_Event event;
  button k;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
    case SDL_KEYDOWN:
      if(event.key.repeat == 0) {
	k.pressed = true;
	k.checked = false;
	keys[event.key.keysym.sym] = k;
      }
      break;

    case SDL_KEYUP:
      k.pressed = false;
      k.checked = false;
      keys[event.key.keysym.sym] = k;
      break;

    case SDL_MOUSEMOTION:
      lastMousePosition.x = event.motion.x;
      lastMousePosition.y = event.motion.y;
      break;

    case SDL_MOUSEBUTTONDOWN:
      mouseButton.pressed = true;
      mouseButton.checked = false;
      break;

    case SDL_MOUSEBUTTONUP:
      mouseButton.pressed = false;
      mouseButton.checked = false;
      break;

    case SDL_QUIT:
      quitEvent = true;
      break;
    }
  }
}

bool
Input::pressed(int key) {
  if(keys[key].pressed && !keys[key].checked) {
    keys[key].checked = true;
    return true;
  }
  return false;
}

bool
Input::released(int key) {
  if(!keys[key].pressed && !keys[key].checked) {
    keys[key].checked = true;
    return true;
  }
  return false;
}

bool
Input::held(int key) {
  return keys[key].pressed;
}

bool
Input::mouseClicked() {
  if(mouseButton.pressed && !mouseButton.checked) {
    mouseButton.checked = true;
    return true;
  }
  return false;
}

bool
Input::mouseReleased() {
  if(!mouseButton.pressed && !mouseButton.checked) {
    mouseButton.checked = true;
    return true;
  }
  return false;
}
