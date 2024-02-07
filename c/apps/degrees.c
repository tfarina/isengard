#include <stdio.h>
#include <stdlib.h>

#include "angle.h"

int
main(int argc, char **argv)
{
  float rad = 0;
  float deg = 0;

  if (argc < 2)
    {
      fputs("usage: degrees <radians value>\n", stderr);
      return 1;
    }

  rad = atof(argv[1]);
  deg = degrees(rad);

  printf("%.6f\n", deg);

  return 0;
}
