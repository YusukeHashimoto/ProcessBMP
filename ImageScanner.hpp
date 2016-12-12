#include "pixel.hpp"
#include "const.hpp"

class ImageScanner {
  public:
  Pixel image[MAX_WIDTH][MAX_HEIGHT];
  int width;
  int height;

  ImageScanner(int width, int height);
  void scanHolizontally(int*);
  void scanVertically(int*);
  void scanDiagonally1(int*);
  void scanDiagonally2(int*);
};
