#include <stdio.h>

#include "hostname.h"

int
main(void)
{
  char *hostname;

  hostname = get_hostname();

  printf("%s\n", hostname);

  return 0;
}
