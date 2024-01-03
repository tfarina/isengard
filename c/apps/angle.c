#include "angle.h"

#ifndef MATH_PI
#define	MATH_PI 3.14159265358979323846
#endif

float
degrees(float radians)
{
  float result;

  result = radians * (180 / MATH_PI);

  return result;
}

float
radians(float degrees)
{
  float result;

  result = degrees * (MATH_PI / 180);

  return result;
}
