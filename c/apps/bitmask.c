#include <stdio.h>

#define BITMASK(index) (1 << (index))

int
main(
  void
  )
{
  /* left-shift operator (<<)
   *
   * What does it do? It tells the compiler to move the bit on the most right
   * to the left by n positions.
   *
   * If you start with 0000 0001, it creates a 'power of 2' effect.
   *
   * In other words, left shifting (<<) an integer "x" with an integer "y"
   * (x << y) is equivalent to multiplying x by 2^y (2 raise to power y).
   */
  printf("%u\n", BITMASK(0)); /* 0000 0001 | 0x01 | 1   */
  printf("%u\n", BITMASK(1)); /* 0000 0010 | 0x02 | 2   */
  printf("%u\n", BITMASK(2)); /* 0000 0100 | 0x04 | 4   */
  printf("%u\n", BITMASK(3)); /* 0000 1000 | 0x08 | 8   */
  printf("%u\n", BITMASK(4)); /* 0001 0000 | 0x10 | 16  */
  printf("%u\n", BITMASK(5)); /* 0010 0000 | 0x20 | 32  */
  printf("%u\n", BITMASK(6)); /* 0100 0000 | 0x40 | 64  */
  printf("%u\n", BITMASK(7)); /* 1000 0000 | 0x80 | 128 */

  return 0;
}
