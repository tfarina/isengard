#include <stdint.h>

int main(void) {
  const uint8_t initial_bits = 0b00001111;
  const uint8_t inverted_bits = ~initial_bits; /* equals to 0b11110000 */

  const uint8_t first_six_bits = 0b11111100;
  const uint8_t last_six_bits  = 0b00111111;
  const uint8_t middle_four_bits = first_six_bits & last_six_bits; /* equals to 0b00111100 */

  return 0;
}
