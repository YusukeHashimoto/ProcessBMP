class Pixel {
  int r;
  int g;
  int b;
public:
  void setRGB(int r, int g, int b);
  unsigned char getR() { return r; }
  unsigned char getG() { return g; }
  unsigned char getB() { return b; }
  unsigned char getY() { return 0.299 * r + 0.587 * g + 0.114 * b; }
};
