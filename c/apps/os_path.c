#include "os_path.h"

#include <string.h>

#define DIR_SEP '/'

char const *os_path_basename(char const *path) {
  char const *last_slash;

  /* Search for the last slash in path. */
  last_slash = strrchr(path, DIR_SEP);
  /* If there is no DIR_SEP in path, the basename is path. */
  if (last_slash == 0) {
    return path;
  }

  return last_slash + 1; /* Move the pointer past the '/'. */
}
