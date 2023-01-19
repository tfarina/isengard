#include <stdio.h>
#include <stdlib.h>

#include "user.h"

int main(int argc, char **argv) {
  char *configdir;

  configdir = user_config_dir();

  if (configdir)
  {
    printf("%s\n", configdir);
    free(configdir);
  }

  return 0;
}
