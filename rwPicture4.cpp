#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const long SIZE = 1000000;
const char* DEFAULT_NAME = "newFile.bmp";
const int BEGINNING_OF_CONTENT = 54;

void sharpen();
int readImage(char*);


unsigned char image[SIZE];

class Header {
  unsigned char binary[SIZE];
  int width;
  int height;
public:
  Header(int size) {

    for(int i = 0; i < BEGINNING_OF_CONTENT; i++) {
      binary[i] = image[i];
    }

    char value[16];
    sprintf(value, "%02x%02x%02x%02x", image[21], image[20], image[19], image[18]);
    width = std::stoi(value, 0, 16);
    sprintf(value, "%02x%02x%02x%02x", image[25], image[24], image[23], image[22]);
    height = std::stoi(value, 0, 16);
  }
  
  int getWidth() { return width; }
  int getHeight(){ return height; }
  unsigned char* getBinary() { return binary; }
};

void writeImage(int, Header);

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
  unsigned char getR() { return r; }
  unsigned char getG() { return g; }
  unsigned char getB() { return b; }
  int getY() {
	return 0.299 * r + 0.587 * g + 0.114 * b;
  }
};

Pixel pixels[5000][5000];

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);
  
  Header header(size);

  cout << header.getWidth() << endl;
  cout << header.getHeight() << endl;
  cout << size << endl;


  for(int y = 0; y < header.getHeight(); y++) {
	for(int x = 0; x < header.getWidth(); x++) {
	  int i = y * header.getWidth() + x;
	  i*=3;
	  pixels[x][y].setRGB(image[i+2], image[i+1], image[i]);
	}
  }
  
  writeImage(size, header);
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

void writeImage(int filesize, Header header) {
  FILE *fp = fopen(DEFAULT_NAME, "wb");
  /*
  for(int i = 0; i < filesize; i++) {
    fputc(image[i], fp);
	}*/
  for(int i = 0; i < BEGINNING_OF_CONTENT; i++) {
	fputc(header.getBinary()[i], fp);
  }

  int width = header.getWidth();
  int height = header.getHeight();
  
  for(int y = 0; y < height; y++) {
	for(int x = 0; x < width; x++) {
	  fputc(pixels[x][y].getB(), fp);
	  fputc(pixels[x][y].getG(), fp);
	  fputc(pixels[x][y].getR(), fp);
	}
  }
  
  fclose(fp);
}

void sharpen() {
}
