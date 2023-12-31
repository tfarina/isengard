#include <stdio.h>

#ifndef MATH_PI
#define	MATH_PI 3.14159265358979323846
#endif

static
float
degrees(float radians)
{
  float result;

  result = radians * (180 / MATH_PI);

  return result;
}

int
main(void)
{
  float rad = 10;
  float deg;

  deg = degrees(rad);

  printf("%.6f\n", deg);

  return 0;
}
