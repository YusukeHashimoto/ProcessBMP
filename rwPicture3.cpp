#include <stdio.h>
#include <iostream>

using namespace std;

const long SIZE = 1000000;
const char* DEFAULT_NAME = "newFile.bmp";
const int BEGINNING_OF_CONTENT = 54;

double limitRange(double);
int readImage(char*);
void writeImage(int);

unsigned char image[SIZE];

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);

  for(int i = BEGINNING_OF_CONTENT; i < size; i += 3) {
    double b = image[i];
    double g = image[i+1];
    double r = image[i+2];
    double y = 0.299 * r + 0.587 * g + 0.114 * b;
    double cb = -0.1687 * r - 0.3313 * g + 0.500 * b;
    double cr = 0.500 * r - 0.4187 * g - 0.0813 * b;
    
    cb -= 50;
    cr -= 50;
    
    r = y + 1.402 * cr;
    g = y - 0.344 * cb - 0.714 * cr;
    b = y + 1.772 * cb;
    
    r = limitRange(r);
    g = limitRange(g);
    b = limitRange(b);
    
    image[i] = b;
    image[i+1] = g;
    image[i+2] = r;
  }

  writeImage(size);
  return 0;
}

double limitRange(double x) {
  if(x > 255) return 255;
  if(x < 0) return 0;
  return x;
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
