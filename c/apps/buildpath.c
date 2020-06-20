#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffileutils.h"

int main(void) {
  char *path;

  path = f_build_filename("/etc", "resolv.conf");

  printf("%s\n", path);

  free(path);

  return 0;
}
