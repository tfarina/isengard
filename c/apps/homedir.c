#include <stdio.h>
#include <stdlib.h>

#include "user.h"

int
main(void)
{
  char *homedir;

  homedir = user_home_dir();

  printf("%s\n", homedir);

  return 0;
}
