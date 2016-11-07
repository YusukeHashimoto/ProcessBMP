#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const long SIZE = 1000000;
const char* DEFAULT_NAME = "newFile.bmp";
const int BEGINNING_OF_CONTENT = 54;

void sharpen();
int readImage(char*);
void writeImage(int);

unsigned char image[SIZE];

class Header {
  unsigned char binary[SIZE];
  int width;
  int height;
public:
  Header() {
    for(int i = 0; i < BEGINNING_OF_CONTENT; i++) {
      binary[i] = image[i];
    }
    char hexValue[8];
    sprintf(hexValue, "%02d%02d%02d%02d", image[21], image[20], image[19], image[18]);
    width = std::stoi(hexValue, 0, 16);
    sprintf(hexValue, "%02d%02d%02d%02d", image[25], image[24], image[23], image[22]);
    height = std::stoi(hexValue, 0, 16);
  }
  int getWidth() { return width; }
  int getHeight(){ return height; }
};

class Pixel {
  int r;
  int g;
  int b;
public:
  void setRGB(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }
  int getR() { return r; }
  int getG() { return g; }
  int getB() { return b; }
  int getY() { return 0; }
};

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);
  
  Header header;
  cout << header.getWidth() << endl;
  cout << header.getHeight() << endl;
  cout << size << endl;

  //  Pixel binary[header.getWidth()][header.getHeight()];
  Pixel p[500][500];
  /*
  int i = 0;
  for(int y = 0; y < header.getHeight(); y++) {
    for(int x = 0; x < header.getWidth(); x++) {
      //binary[x][y].setRGB(i+2, i+1, i);
      i += 3;
      cout << "(" << x << ", " << y << ")" << endl;
    }
  }
  */
  writeImage(size);
  return 0;
}

int readImage(char* filename) {
  FILE *fp = fopen(filename, "rb");
  int i = 0;
  for(int c; (c = fgetc(fp)) != EOF; i++) {
    image[i] = c;
  }
  fclose(fp);
  return i;
}

void writeImage(int filesize) {
  FILE *fp = fopen(DEFAULT_NAME, "wb");
  for(int i = 0; i < filesize; i++) {
    fputc(image[i], fp);
  }
  fclose(fp);
}

void sharpen() {
}
