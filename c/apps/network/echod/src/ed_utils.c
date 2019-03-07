#include "ed_utils.h"

#include <stdint.h>

int ed_valid_port(int port) {
  if (port < 1 || port > UINT16_MAX) {
    return 0;
  }

  return 1;
}
