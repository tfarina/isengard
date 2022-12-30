#include <stdio.h>
#include <stdlib.h>

#include "os.h"

int
main(void)
{
  char *homedir;

  homedir = os_homedir();

  printf("%s\n", homedir);

  return 0;
}
