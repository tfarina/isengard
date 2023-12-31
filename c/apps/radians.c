#include <stdio.h>

#ifndef MATH_PI
#define	MATH_PI 3.14159265358979323846
#endif

static
float
radians(float degrees)
{
  float result;

  result = degrees * (MATH_PI / 180);

  return result;
}

int
main(void)
{
  float deg = 540;
  float rad;

  rad = radians(deg);

  printf("%.6f\n", rad);

  return 0;
}
