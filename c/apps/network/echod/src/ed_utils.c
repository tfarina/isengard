#include "ed_utils.h"

int ed_valid_port(int port) {
  if (port < 0 || port > 65535) {
    return 0;
  }

  return 1;
}
