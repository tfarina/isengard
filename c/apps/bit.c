#include <stdint.h>
#include <stdio.h>

#define BITMASK(index) (1 << (index))

int main(void) {
  /* Power of two values                 Binary    | Decimal */
  const uint8_t flag1       = 0x01; /* 0b0000 0001 | 1       */
  const uint8_t flag2       = 0x02; /* 0b0000 0010 | 2       */
  const uint8_t flag3       = 0x04; /* 0b0000 0100 | 4       */
  const uint8_t flag4       = 0x08; /* 0b0000 1000 | 8       */
  const uint8_t flag5       = 0x10; /* 0b0001 0000 | 16      */
  const uint8_t flag6       = 0x20; /* 0b0010 0000 | 32      */
  const uint8_t flag7       = 0x40; /* 0b0100 0000 | 64      */
  const uint8_t flag8       = 0x80; /* 0b1000 0000 | 128     */

  printf("%d\n", flag1);
  printf("%d\n", flag2);
  printf("%d\n", flag3);
  printf("%d\n", flag4);
  printf("%d\n", flag5);
  printf("%d\n", flag6);
  printf("%d\n", flag7);
  printf("%d\n", flag8);

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
