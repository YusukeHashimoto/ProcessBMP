#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const long SIZE = 1000000;
const char* DEFAULT_NAME = "newFile.bmp";
const int BEGINNING_OF_CONTENT = 54;

int readImage(char*);
void writeImage(int);

unsigned char image[SIZE];

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);

  for(int i = BEGINNING_OF_CONTENT; i < size; i += 3) {
    int b = image[i];
    int g = image[i+1];
    int r = image[i+2];
    double y = 0.299 * r + 0.587 * g + 0.114 * b;

    image[i] = y / 2;
    image[i+1] = y / 2;
    image[i+2] = y / 2;
  }

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
