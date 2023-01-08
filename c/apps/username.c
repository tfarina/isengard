#include <stdio.h>

#include "id.h"

int
main(void)
{
  char *username;

  username = id_username();

  printf("%s\n", username);

  return 0;
}
