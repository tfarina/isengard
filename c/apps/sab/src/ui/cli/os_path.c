#include "os_path.h"

#include <string.h>

#define DIR_SEP '/'

char const *os_path_basename(char const *path) {
  char const *last_slash;

  /* Search for the last slash in path. */
  last_slash = strrchr(path, DIR_SEP);
  if (last_slash == 0) {
    return path;
  }

  return last_slash + 1; /* move pointer to char after '/' */
}
