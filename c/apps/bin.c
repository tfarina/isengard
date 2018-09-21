#include <stdio.h>

#include "math_util.h"

/**
 * To print binary representation of unsigned integer, start from 31th bit,
 * check wheter 31th bit is ON or OFF, if it is ON print "1" else print "0".
 * Do this for all bits from 31 to 0.
 *
 * Code from: www.geeksforgeeks.org/binary-representation-of-a-given-number
 */
static void dec2bin(unsigned n) {
  unsigned i;
  for (i = 1 << 31; i > 0; i = i / 2) {
    (n & i) ? printf("1") : printf("0");
  }
}

int main(int argc, char **argv) {
  long int five = 0b101;

  dec2bin(120);
  printf("\n");

  printf("%ld\n", bin2dec(five));

  return 0;
}
