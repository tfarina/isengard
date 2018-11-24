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
 * New code from: https://www.programiz.com/c-programming/examples/binary-decimal-convert
 */
long long dec2bin(unsigned n) {
  long long binaryNumber = 0;
  int remainder, i = 1, step = 1;

  while (n!=0)
    {
      remainder = n%2;
      //printf("Step %d: %d/2, Remainder = %d, Quotient = %d\n", step++, n, remainder, n/2);
      n /= 2;
      binaryNumber += remainder*i;
      i *= 10;
    }
  return binaryNumber;
}
