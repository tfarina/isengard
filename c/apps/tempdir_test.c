#include "tempdir.h"

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  char *tempdir = NULL;

  tempdir = f_tempdir();

  if (tempdir != NULL) {
    printf("%s\n", tempdir);

    free(tempdir);
  }

  return 0;
}
