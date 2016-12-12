#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

//#include "const.hpp"
#include "ImageScanner.hpp"
#include "header.hpp"
//#include "pixel.hpp"

using namespace std;

int readImage(char*);
int limitRange(int);
void writeImage(int, Header);
//void execReverseProjection(int, int, int[], int[]);
void execReverseProjection(int, int, int[], int[], int[], int[]);

Header::Header(int size, unsigned char image[]) {
  for(int i = 0; i < SIZE_OF_HEADER; i++) {
    binary[i] = image[i];
  }

  char value[16];
  sprintf(value, "%02x%02x%02x%02x", image[21], image[20], image[19], image[18]);
  //width = std::stoi(value, 0, 16);
  width = strtoll(value, 0, 16);
  sprintf(value, "%02x%02x%02x%02x", image[25], image[24], image[23], image[22]);
  //height = std::stoi(value, 0, 16);
  height = strtoll(value, 0, 16);
}

void Pixel::setRGB(int r, int g, int b) {
  this->r = limitRange(r);
  this->g = limitRange(g);
  this->b = limitRange(b);
}

unsigned char rawImage[MAX_SIZE];
Pixel pixels[MAX_WIDTH][MAX_HEIGHT];
Pixel sharpenedPixels[MAX_WIDTH][MAX_HEIGHT];
double k = 1;

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);
  if(argc > 2) k = atoi(argv[2]);

  Header header(size, rawImage);

  for(int y = 0; y < header.getHeight(); y++) {
    for(int x = 0; x < header.getWidth(); x++) {
      int i = SIZE_OF_HEADER + (y * header.getWidth() + x) * 3;
      pixels[x][y].setRGB(rawImage[i+2], rawImage[i+1], rawImage[i]);
    }
  }

  int width = header.getWidth();
  int height = header.getHeight();

  ImageScanner *scanner = new ImageScanner(width, height);
  
  int holResult[height] = {0};
  scanner->scanHolizontally(holResult);
  int verResult[width] = {0};
  scanner->scanVertically(verResult);
  
  int diaResult1[width+height-1] = {0};
  scanner->scanDiagonally1(diaResult1);
  int diaResult2[width+height-1] = {0};
  scanner->scanDiagonally2(diaResult2);
  
  //execReverseProjection(width, height, holResult, verResult);
  execReverseProjection(width, height, holResult, verResult, diaResult1, diaResult2);
  //cout << width << endl;
  writeImage(size, header);
  return 0;
}

int readImage(char* filename) {
  FILE *fp = fopen(filename, "rb");
  int i = 0;
  for(int c; (c = fgetc(fp)) != EOF; i++) {
    rawImage[i] = c;
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

  cout << width << " " << height << endl;
  cout << width * height * 3<< endl << filesize << endl;

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      fputc(sharpenedPixels[x][y].getB(), fp);//b
      fputc(sharpenedPixels[x][y].getG(), fp);//g
      fputc(sharpenedPixels[x][y].getR(), fp);//r
      /*
        cout << "(" << (int)sharpenedPixels[x][y].getR()
        << "," << (int)sharpenedPixels[x][y].getG()
        << "," << (int)sharpenedPixels[x][y].getB() << ")" << endl;
      */
    }
  }
  fclose(fp);
}

int limitRange(int n) {
  if(n < 0) return 0;
  if(n > 255) return 255;
  return n;
}

ImageScanner::ImageScanner(int width, int height) {
  this->width = width;
  this->height = height;

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      this->image[i][j] = pixels[i][j];
      //this->image[i][j].setRGB(0, 0, 0);
    }
  }
}

void ImageScanner::scanHolizontally(int result[]) {
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      //result[y] += image[x][y].getY();
      if(result[y] < image[x][y].getY())
        result[y] = image[x][y].getY();
    }
  }
 
}

//
void ImageScanner::scanVertically(int result[]) {
  for(int x = 0; x < width; x++) {      
    for(int y = 0; y < height; y++) {
      if(result[x] < image[x][y].getY()) {
        result[x] = image[x][y].getY();
      }
    }
    //cout << result[x] << " ";
  }
}

void ImageScanner::scanDiagonally1(int result[]) {
  for(int x = 0; x < width+height; x++) {
    for(int j = 0; j < x+1; j++) {
      if(x-j < width && result[x] < image[x-j][j].getY()) {
        result[x] = image[x-j][j].getY();
      }
    }
  }/*
  for(int y = 1; y < height; y++) {
    for(int x = width; x > 1; x--) {
      if(result[width+y-1] < image[x][y].getY()){                                            result[width+y-1] = image[x][y].getY();
      }
    }
    }*/
}

void ImageScanner::scanDiagonally2(int result[]) {
  for(int x = 0; x < width; x++) {
    for(int j = 0; j < x+1 && j < height; j++) {
      if(result[x] < image[x-j][height-j].getY())
        result[x] = image[x-j][height-j].getY();
    }
  }
}


//void execReverseProjection(int width, int height, int holResult[], int verResult[]) {
void execReverseProjection(int width, int height, int holResult[], int verResult[], int diaResult1[], int diaResult2[]) {
  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      int value = (holResult[y] + verResult[x] + diaResult1[x+y] + diaResult2[x+height-y]) / 4;
      value = limitRange(value);
      sharpenedPixels[x][y].setRGB(value, value, value);
    }
  }
}

int max(int a, int b) {
  if(a > b) return a;
  else return b;
}
