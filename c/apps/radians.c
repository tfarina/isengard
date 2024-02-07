#include <stdio.h>
#include <stdlib.h>

#include "angle.h"

int
main(int argc, char **argv)
{
  float deg = 0;
  float rad = 0;

  if (argc < 2)
    {
      fputs("usage: radians <degrees value>\n", stderr);
      return 1;
    }

  deg = atof(argv[1]);
  rad = radians(deg);

  printf("%.6f\n", rad);

  return 0;
}
