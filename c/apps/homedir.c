#include <stdio.h>
#include <stdlib.h>

#include "fileutil.h"

int main(int argc, char **argv) {
  const char *homedir;

  homedir = f_get_home_dir();

  printf("%s\n", homedir);

  return 0;
}
