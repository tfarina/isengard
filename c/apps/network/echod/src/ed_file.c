#include "ed_file.h"

#include <string.h>

#define ED_DIR_SEP '/'

char const *ed_basename(char const *path) {
  char const *name;

  name = strrchr(path, ED_DIR_SEP);
  if (name == NULL) {
    name = path;
  } else {
    name++; /* move pointer to char after '/' */
  }

  return name;
}
