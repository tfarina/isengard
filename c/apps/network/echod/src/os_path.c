#include "os_path.h"

#include <string.h>

#define DIR_SEP '/'

char const *os_path_basename(char const *path) {
  char const *slash;

  slash = strrchr(path, DIR_SEP);
  if (slash == NULL) {
    return path;
  }

  return slash + 1; /* move pointer to char after '/' */
}
