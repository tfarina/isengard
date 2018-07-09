/*
 * Code from: www.geeksforgeeks.org/binary-representation-of-a-given-number
 */

#include <math.h>
#include <stdio.h>

/* To print binary representation of unsigned integer, start from 31th bit,
 * check wheter 31th bit is ON or OFF, if it is ON print "1" else print "0".
 * Do this for all bits from 31 to 0.
 */
static void bin(unsigned n) {
  unsigned i;
  for (i = 1 << 31; i > 0; i = i / 2) {
    (n & i) ? printf("1") : printf("0");
  }
}

/**
 * Converts a binary number to decimal.
 *
 * Algorithm from http://www.c4learn.com/c-programs/program-to-convert-binary-to-decimal.html
 *
 */
static long int bin2dec(long int num) {
  long int rem, sum = 0, power = 0;

  while (num > 0) {
    rem = num % 10;
    num = num / 10;
    sum = sum + rem * pow(2, power);
    power++;
  }

  return sum;
}

int main(int argc, char **argv) {
  long int five = 0b101;

  bin(120);
  printf("\n");

  printf("%ld\n", bin2dec(five));

  return 0;
}
