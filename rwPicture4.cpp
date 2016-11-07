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
  Header(int size) {

    for(int i = 0; i < size; i++) {
      binary[i] = image[i + BEGINNING_OF_CONTENT];
    }

    char value[16];
    sprintf(value, "%02d%02d%02d%02d", image[21], image[20], image[19], image[18]);
    width = std::stoi(value, 0, 16);
		  
    sprintf(value, "%02d%02d%02d%02d", image[25], image[24], image[23], image[22]);
    height = std::stoi(value, 0, 16);
  }
  
  int getWidth() { return width; }
  int getHeight(){ return height; }
};

int main(int argc, char* argv[]) {
  int size = readImage(argv[1]);
  
  Header header(size);
  cout << header.getWidth() << endl;
  cout << header.getHeight() << endl;
  cout << size << endl;
  
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
/*
class Header {
  unsigned char binary[BEGINNING_OF_CONTENT];
  //int width;
  int height;

public:
  //Header(unsigned char image[SIZE]) {
  Header() {
    for(int i = 0; i < BEGINNING_OF_CONTENT; i++) {
      binary[i] = image[i];
    }
    char width[8];
    sprintf(width, "%02d%02d%02d%02d", image[21], image[20], image[19], image[18]);
    //cout << stoi(width, 0, 16) << endl;
  }
  long getHeight() { return height; }
  //long getWidth() { return width; }
};

class Sample {
public:

};

*/
