#include <stdio.h>

#include "os.h"

int
main(void)
{
  char *hostname;

  hostname = os_hostname();
  if (!hostname) {
    return 1;
  }

  printf("%s\n", hostname);

  return 0;
}
