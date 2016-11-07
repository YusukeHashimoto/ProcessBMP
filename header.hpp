class Header {
  unsigned char binary[MAX_SIZE];
  int width;
  int height;
public:
  Header(int size, unsigned char image[]);
  int getWidth() { return width; }
  int getHeight(){ return height; }
  unsigned char* getBinary() { return binary; }
};
