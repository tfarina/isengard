#include <stdint.h>
#include <stdio.h>

#define BITMASK(index) (1 << (index))

int main(void) {
  const uint8_t initial_bits = 0b00001111;
  const uint8_t inverted_bits = ~initial_bits; /* equals to 0b11110000 */

  const uint8_t first_six_bits = 0b11111100;
  const uint8_t last_six_bits  = 0b00111111;
  const uint8_t middle_four_bits = first_six_bits & last_six_bits; /* equals to 0b00111100 */

  const uint8_t some_bits = 0b10110010;
  const uint8_t more_bits = 0b01011110;
  const uint8_t combined_bits = some_bits | more_bits; /* equals to 0b11111110 */ 

  /* Powers of 2 in binary */
  const uint8_t zero                  = 0b00000000;
  const uint8_t one                   = 0b00000001;
  const uint8_t two                   = 0b00000010;
  const uint8_t four                  = 0b00000100;
  const uint8_t eight                 = 0b00001000;
  const uint8_t sixteen               = 0b00010000;
  const uint8_t thirtytwo             = 0b00100000;
  const uint8_t sixtyfour             = 0b01000000;
  const uint8_t onehundredtwentyeight = 0b10000000;

  printf("%d\n", zero);
  printf("%d\n", one);
  printf("%d\n", two);
  printf("%d\n", four);
  printf("%d\n", eight);
  printf("%d\n", sixteen);
  printf("%d\n", thirtytwo);
  printf("%d\n", sixtyfour);
  printf("%d\n", onehundredtwentyeight);

  return 0;
}
