#include <stdio.h>
#include <stdlib.h>

#include "futils.h"

int main(int argc, char **argv) {
  char *homedir;

  homedir = f_get_home_dir();

  printf("%s\n", homedir);

  return 0;
}
