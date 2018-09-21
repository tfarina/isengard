#include <stdio.h>

#include "math_util.h"

int main(int argc, char **argv) {
  long int five = 0b101;

  dec2bin(120);
  printf("\n");

  printf("%ld\n", bin2dec(five));

  return 0;
}
