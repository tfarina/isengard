#include <stdint.h>

int main(void) {
  const uint8_t initial_bits = 0b00001111;
  const uint8_t inverted_bits = ~initial_bits; /* equals to 0b11110000 */

  return 0;
}
