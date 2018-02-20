/**
 * Pivot point = P = (H + L + C) / 3
 * First area of resistance = R1 = 2P - L
 * Second area of resistance = R2 = P + (R1 - S1)
 * Third area of resistance = R3 = H + 2 * (P - L)
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

static float resistance2(float pivot_point, float r1, float s1)
{
  return pivot_point + (r1 - s1);
}

static float resistance3(float pivot_point, float high, float low)
{
  return high + 2 * (pivot_point - low);
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
  float r2;
  float r3;
  float s1;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  pp = pivot_point(high, low, close);
  s1 = support1(pp, high);
  r1 = resistance1(pp, low);
  r2 = resistance2(pp, r1, s1);
  r3 = resistance3(pp, high, low);

  printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", s1, pp, r1, r2, r3);

  return 0;
}
