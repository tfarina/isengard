#include "math_util.h"

#include <math.h>

long int bin2dec(long int num) {
  long int rem, sum = 0, power = 0;

  while (num > 0) {
    rem = num % 10;
    num = num / 10;
    sum = sum + rem * pow(2, power);
    power++;
  }

  return sum;
}

/**
 * To print binary representation of unsigned integer, start from 31th bit,
 * check wheter 31th bit is ON or OFF, if it is ON print "1" else print "0".
 * Do this for all bits from 31 to 0.
 *
 * Code from: www.geeksforgeeks.org/binary-representation-of-a-given-number
 */
void dec2bin(unsigned n) {
  unsigned i;
  for (i = 1 << 31; i > 0; i = i / 2) {
    (n & i) ? printf("1") : printf("0");
  }
}
