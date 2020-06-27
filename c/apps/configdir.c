#include <stdio.h>
#include <stdlib.h>

#include "ffileutils.h"
#include "futils.h"

int main(int argc, char **argv) {
  char *homedir;
  char *configdir;

  homedir = f_get_home_dir();
  configdir = f_build_filename(homedir, ".config");

  printf("%s\n", configdir);

  free(configdir);

  return 0;
}
