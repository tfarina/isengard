#include "ec_utils.h"

#include <string.h>

#define EC_DIR_SEP '/'

char *ec_get_progname(char *argv0) {
  char *name;

  name = strrchr(argv0, EC_DIR_SEP);
  if (name == NULL) {
    name = argv0;
  } else {
    name++;
  }

  return name;
}

