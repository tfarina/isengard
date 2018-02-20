/**
 * Pivot point = P = (H + L + C) / 3
 * First area of resistance = R1 = 2P - L
 * First area of support = S1 = 2P - H
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

static float support1(float pivot_point, float high)
{
  return (2 * pivot_point) - high;
}

int main(int argc, char **argv)
{
  float high, low, close;
  float pp;
  float r1;
  float s1;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  pp = pivot_point(high, low, close);
  r1 = resistance1(pp, low);
  s1 = support1(pp, high);

  printf("%.4f\t%.4f\t%.4f\n", s1, pp, r1);

  return 0;
}
