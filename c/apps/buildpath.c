#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os_path.h"

int main(void) {
  char *path;

  path = os_path_join("/etc", "resolv.conf");

  printf("%s\n", path);

  free(path);

  return 0;
}
