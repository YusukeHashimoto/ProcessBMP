#include <stdio.h>
#define SIZE 1000000

int main(int argc, char* argv[]) {
  unsigned char image[SIZE];
  FILE *fp;
  int c;
  int i = 0;
  int size;
  char *filename;

  //read an input file
  
  fp = fopen(argv[1], "rb");
  
  while((c = fgetc(fp)) != EOF) {
    image[i++] = c;
  }

  size = i;
  fclose(fp);

  //write a new file
  
  fp = fopen(argc > 2 ? argv[2] : "newfile.bmp", "wb");
  for(i = 0; i < size; i++) {
    fputc(image[i], fp);
  }
  fclose(fp);
  
  return 0;
}
