/**
 * Pivot point = P = (H + L + C) / 3
 */

#include <stdio.h>

static float pivot_point(float high, float low, float close)
{
  return (high + low + close) / 3;
}

static float resistance1(float pivot_point, float low)
{
  return (2 * pivot_point) - low;
}

int main(int argc, char **argv)
{
  float high, low, close;
  float pp;
  float r1;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  pp = pivot_point(high, low, close);
  r1 = resistance1(pp, low);

  printf("%.4f\t%.4f\n", pp, r1);

  return 0;
}
