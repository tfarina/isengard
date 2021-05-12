#include <stdio.h>

#include "pivot.h"

int main(int argc, char **argv)
{
  float high, low, close;
  float pp;
  float r1;
  float r2;
  float r3;
  float s1;
  float s2;
  float s3;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  pp = classic_pp_pivot_point(high, low, close);
  s1 = classic_pp_support1(pp, high);
  r1 = classic_pp_resistance1(pp, low);
  r2 = classic_pp_resistance2(pp, high, low);
  r3 = classic_pp_resistance3(pp, high, low);
  s2 = classic_pp_support2(pp, high, low);
  s3 = classic_pp_support3(pp, high, low);

  printf("Close\tHigh\tLow\n");
  printf("%.2f\t%.2f\t%.2f\n\n", close, high, low);

  printf("S3: %.4f\nS2: %.4f\nS1: %.4f\nPP: %.4f\nR1: %.4f\nR2: %.4f\nR3: %.4f\n", s3, s2, s1, pp, r1, r2, r3);

  return 0;
}
