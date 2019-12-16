#include "ed_utils.h"

#include <stdint.h>
#include <string.h>

#define ED_DIR_SEP '/'

char const *ed_basename(char const *path) {
  char const *name;

  name = strrchr(path, ED_DIR_SEP);
  if (name == NULL) {
    name = path;
  } else {
    name++;
  }

  return name;
}

int ed_valid_port(int port) {
  if (port < 1 || port > UINT16_MAX) {
    return 0;
  }

  return 1;
}
