#include <stdint.h>
#include <stdio.h>

#define BITMASK(index) (1 << (index))

int main(void) {
  /* Powers of 2 in binary */
  const uint8_t zero                  = 0x00; /* 0b0000 0000 */
  const uint8_t one                   = 0x01; /* 0b0000 0001 */
  const uint8_t two                   = 0x02; /* 0b0000 0010 */
  const uint8_t four                  = 0x04; /* 0b0000 0100 */
  const uint8_t eight                 = 0x08; /* 0b0000 1000 */
  const uint8_t sixteen               = 0x10; /* 0b0001 0000 */
  const uint8_t thirtytwo             = 0x20; /* 0b0010 0000 */
  const uint8_t sixtyfour             = 0x40; /* 0b0100 0000 */
  const uint8_t onehundredtwentyeight = 0x80; /* 0b1000 0000 */

  printf("%d\n", zero);
  printf("%d\n", one);
  printf("%d\n", two);
  printf("%d\n", four);
  printf("%d\n", eight);
  printf("%d\n", sixteen);
  printf("%d\n", thirtytwo);
  printf("%d\n", sixtyfour);
  printf("%d\n", onehundredtwentyeight);

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
