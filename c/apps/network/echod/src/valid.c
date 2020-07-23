#include "valid.h"

int valid_port(int port) {
  if (port < 1 || port > 65535) {
    return 0;
  }

  return 1;
}
