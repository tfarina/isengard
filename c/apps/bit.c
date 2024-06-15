#include <stdio.h>

/*                  Power of 2 values      */

/*             Hex     Binary    | Decimal */
#define FLAG01 0x01 /*         1 | 1       */
#define FLAG02 0x02 /*        10 | 2       */
#define FLAG03 0x04 /*       100 | 4       */
#define FLAG04 0x08 /*      1000 | 8       */
#define FLAG05 0x10 /*    1 0000 | 16      */
#define FLAG06 0x20 /*   10 0000 | 32      */
#define FLAG07 0x40 /*  100 0000 | 64      */
#define FLAG08 0x80 /* 1000 0000 | 128     */

#define BITMASK(index) (1 << (index))

int main(void) {

  /* This will print the decimal values from the hexadecimal constants
   */
  printf("%u\n", FLAG01);
  printf("%u\n", FLAG02);
  printf("%u\n", FLAG03);
  printf("%u\n", FLAG04);
  printf("%u\n", FLAG05);
  printf("%u\n", FLAG06);
  printf("%u\n", FLAG07);
  printf("%u\n", FLAG08);

  /*
   * left-shift operator (<<)
   *
   * What does it do? It tells the compiler to move the bit on the most right
   * to the left by n positions.
   *
   * If you start with 0000 0001, it creates a 'power of 2' effect.
   *
   * In other words, left shifting (<<) an integer "x" with an integer "y"
   * (x << y) is equivalent to multiplying x by 2^y (2 raise to power y).
   */
  printf("%d\n", BITMASK(0)); /* 0000 0001 | 0x01 | 1   */
  printf("%d\n", BITMASK(1)); /* 0000 0010 | 0x02 | 2   */
  printf("%d\n", BITMASK(2)); /* 0000 0100 | 0x04 | 4   */
  printf("%d\n", BITMASK(3)); /* 0000 1000 | 0x08 | 8   */
  printf("%d\n", BITMASK(4)); /* 0001 0000 | 0x10 | 16  */
  printf("%d\n", BITMASK(5)); /* 0010 0000 | 0x20 | 32  */
  printf("%d\n", BITMASK(6)); /* 0100 0000 | 0x40 | 64  */
  printf("%d\n", BITMASK(7)); /* 1000 0000 | 0x80 | 128 */

  return 0;
}
