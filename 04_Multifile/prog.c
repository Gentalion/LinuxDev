#include "outlib.h"
int main(int argc, char *argv[]) {
       Count = argc;
 output("<INIT>");
     output(argc>1 ? argv[1] : "<NOPE>");
  output("<DONE>");
     return 0;
}