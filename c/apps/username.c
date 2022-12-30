#include <stdio.h>

#include "os.h"

int
main(void)
{
  char *username;

  username = os_username();

  printf("%s\n", username);

  return 0;
}
