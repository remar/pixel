#include "Image.h"

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::~Image() {
  clear();
}

void
Image::clear() {
  if(pixels) {
    delete[] pixels;
  }
  pixels = nullptr;
  width = 0;
  height = 0;
}

void
Image::loadPng(std::string filename) {
  int w, h, n;
  unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, 0);
  assert(n == 4);
  if(data) {
    clear();
    pixels = new uint32_t[w*h];
    width = w;
    height = h;

    uint8_t *data_u8 = data;
    for(int i = 0;i < w*h;i++) {
      uint8_t r = *data_u8;
      data_u8++;
      uint8_t g = *data_u8;
      data_u8++;
      uint8_t b = *data_u8;
      data_u8++;
      uint8_t a = *data_u8;
      data_u8++;

      pixels[i] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
    }

    stbi_image_free(data);
  }
}
