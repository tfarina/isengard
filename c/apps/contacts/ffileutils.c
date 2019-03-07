#include "ffileutils.h"

#include <stdlib.h>
#include <string.h>

#define PATH_SEP '/'

/* Code from msmtp:src/tools.c:get_filename */
char *f_build_filename(const char *directory, const char *name)
{
  char *path;
  size_t dirlen;

  dirlen = strlen(directory);
  path = malloc((dirlen + strlen(name) + 2) * sizeof(char));
  strcpy(path, directory);
  if (dirlen == 0 || path[dirlen - 1] != PATH_SEP) {
    path[dirlen++] = PATH_SEP;
  }
  strcpy(path + dirlen, name);
  return path;
}
