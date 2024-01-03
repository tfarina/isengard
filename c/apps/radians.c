#include <stdio.h>

#include "angle.h"

int
main(void)
{
  float deg = 540;
  float rad;

  rad = radians(deg);

  printf("%.6f\n", rad);

  return 0;
}
