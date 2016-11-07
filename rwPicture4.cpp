#include <stdio.h>
#include <iostream>
#include <string>

#include "const.hpp"
#include "header.hpp"
#include "pixel.hpp"

using namespace std;

int readImage(char*);
int limitRange(int);
void writeImage(int, Header);
void sharpen(Header);

Header::Header(int size, unsigned char image[]) {
  for(int i = 0; i < SIZE_OF_HEADER; i++) {
      binary[i] = image[i];
    }
  
  char value[16];
  sprintf(value, "%02x%02x%02x%02x", image[21], image[20], image[19], image[18]);
  width = std::stoi(value, 0, 16);
  sprintf(value, "%02x%02x%02x%02x", image[25], image[24], image[23], image[22]);
  height = std::stoi(value, 0, 16);
}

void Pixel::setRGB(int r, int g, int b) {
  this->r = limitRange(r);
  this->g = limitRange(g);
  this->b = limitRange(b);
}

unsigned char image[MAX_SIZE];
Pixel pixels[MAX_WIDTH][MAX_HEIGHT];
Pixel sharpenedPixels[MAX_WIDTH][MAX_HEIGHT];
double k = 1;

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);
  if(argc > 2) k = atoi(argv[2]);
  
  Header header(size, image);

  for(int y = 0; y < header.getHeight(); y++) {
	for(int x = 0; x < header.getWidth(); x++) {
	  int i = SIZE_OF_HEADER + (y * header.getWidth() + x) * 3;
	  pixels[x][y].setRGB(image[i+2], image[i+1], image[i]);
	}
  }

  sharpen(header);
  
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
  
  for(int i = 0; i < SIZE_OF_HEADER; i++) {
	fputc(header.getBinary()[i], fp);
  }

  int width = header.getWidth();
  int height = header.getHeight();
  
  for(int y = 0; y < height; y++) {
	for(int x = 0; x < width; x++) {
	  fputc(sharpenedPixels[x][y].getB(), fp);
	  fputc(sharpenedPixels[x][y].getG(), fp);
	  fputc(sharpenedPixels[x][y].getR(), fp);
	}
  }
  fclose(fp);
}

void sharpen(Header header) {
  int width = header.getWidth();
  int height = header.getHeight();
  
  for(int y = 0; y < height; y++) {
	for(int x = 0; x < width; x++) {
	  if(y == 0 || y == height-1 || x == 0 || x == width-1) {
		sharpenedPixels[x][y].setRGB(pixels[x][y].getR(),
									 pixels[x][y].getG(),
									 pixels[x][y].getB());
	  } else {
		double r = (4.0 * pixels[x][y].getR() * k) + 1.0
		  - (pixels[x-1][y].getR() * k)
		  - (pixels[x+1][y].getR() * k)
		  - (pixels[x][y-1].getR() * k)
		  - (pixels[x][y+1].getR() * k)
		  + pixels[x][y].getR();
		  
		double g = 4.0 * pixels[x][y].getG() * k + 1.0
		  - pixels[x-1][y].getG() * k
		  - pixels[x+1][y].getG() * k
		  - pixels[x][y-1].getG() * k
		  - pixels[x][y+1].getG() * k
		  + pixels[x][y].getG();
		
		double b = 4.0 * pixels[x][y].getB() * k + 1.0
		  - pixels[x-1][y].getB() * k
		  - pixels[x+1][y].getB() * k
		  - pixels[x][y-1].getB() * k
		  - pixels[x][y+1].getB() * k
		  + pixels[x][y].getB();

		sharpenedPixels[x][y].setRGB(limitRange(r), limitRange(g), limitRange(b));
	  }
	}
  }
}

int limitRange(int n) {
  if(n < 0) return 0;
  if(n > 255) return 255;
  return n;
}
