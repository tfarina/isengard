#include <stdio.h>

#include "third_party/xgethostname/xgethostname.h"

int
main(void)
{
  char *hostname;

  hostname = xgethostname();
  if (!hostname) {
    return 1;
  }

  printf("%s\n", hostname);

  return 0;
}
