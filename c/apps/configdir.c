#include <stdio.h>
#include <stdlib.h>

#include "futils.h"

int main(int argc, char **argv) {
  char *configdir;

  configdir = f_get_user_config_dir();

  printf("%s\n", configdir);

  free(configdir);

  return 0;
}
