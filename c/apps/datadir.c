#include <stdio.h>
#include <stdlib.h>

#include "user.h"

int main(int argc, char **argv) {
  char *datadir;

  datadir = user_data_dir();

  if (datadir)
  {
    printf("%s\n", datadir);
    free(datadir);
  }

  return 0;
}
