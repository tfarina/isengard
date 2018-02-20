/**
 * Pivot point = P = (H + L + C) / 3
 */

#include <stdio.h>

static float pivot_point(float high, float low, float close)
{
  return (high + low + close) / 3;
}

int main(int argc, char **argv)
{
  float high, low, close;
  float ppoint;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  ppoint = pivot_point(high, low, close);

  printf("%.4f\n", ppoint);

  return 0;
}
