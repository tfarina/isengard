#include <stdio.h>

#include "os.h"

int
main(void)
{
  char *hostname;

  hostname = os_hostname();

  printf("%s\n", hostname);

  return 0;
}
