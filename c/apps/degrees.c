#include <stdio.h>

#include "angle.h"

int
main(void)
{
  float rad = 10;
  float deg;

  deg = degrees(rad);

  printf("%.6f\n", deg);

  return 0;
}
