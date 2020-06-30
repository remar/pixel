#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
public:
  Image() : Image("") {}
  Image(std::string filename): pixels{nullptr}, width{0}, height{0} {
    if(filename != "") {
      loadPng(filename);
    }
  }
  ~Image();

  void clear();
  bool empty() {return pixels == nullptr;}
  void loadPng(std::string filename);

  uint32_t *pixels;
  int width;
  int height;
};

#endif
