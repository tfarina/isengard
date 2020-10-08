#include "check.h"

#include <stdint.h>

int is_valid_port(int port) {
  if (port < 1 || port > UINT16_MAX) {
    return 0;
  }

  return 1;
}
