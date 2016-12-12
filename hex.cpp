#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

int main() {
  char *s = "b3";
  int i = strtoll(s, 0, 16);
  cout << i << endl;
  
  return 0;
}
