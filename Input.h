#ifndef INPUT_H
#define INPUT_H

#include <map>
#include "Point.h"

class Input {
public:
  void update();
  bool pressed(int key);
  bool released(int key);
  bool held(int key);
  bool quit() {return quitEvent;}
  Point mousePosition() {return lastMousePosition;}
  bool mouseClicked();
  bool mouseReleased();

private:
  struct button {
    bool pressed;
    bool checked;
  };

  std::map<int,button> keys;
  bool quitEvent = false;
  Point lastMousePosition = {0, 0};
  button mouseButton = {};
};

#endif
