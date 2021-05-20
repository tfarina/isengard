#include <stdio.h>

#include "pivot.h"

int main(int argc, char **argv)
{
  double high, low, close;
  double cl_pp, cl_r1, cl_r2, cl_r3, cl_s1, cl_s2, cl_s3;
  double fi_pp, fi_r1, fi_r2, fi_r3, fi_s1, fi_s2, fi_s3;

  high = 60.82;
  low = 59.74;
  close = 60.16;

  cl_pp = pivot_classic_pp(high, low, close);
  cl_r1 = pivot_classic_r1(cl_pp, low);
  cl_r2 = pivot_classic_r2(cl_pp, high, low);
  cl_r3 = pivot_classic_r3(cl_pp, high, low);
  cl_s1 = pivot_classic_s1(cl_pp, high);
  cl_s2 = pivot_classic_s2(cl_pp, high, low);
  cl_s3 = pivot_classic_s3(cl_pp, high, low);

  fi_pp = pivot_fibonacci_pp(high, low, close);
  fi_r1 = pivot_fibonacci_r1(fi_pp, high, low);
  fi_r2 = pivot_fibonacci_r2(fi_pp, high, low);
  fi_r3 = pivot_fibonacci_r3(fi_pp, high, low);
  fi_s1 = pivot_fibonacci_s1(fi_pp, high, low);
  fi_s2 = pivot_fibonacci_s2(fi_pp, high, low);
  fi_s3 = pivot_fibonacci_s3(fi_pp, high, low);

  printf("Close\tHigh\tLow\n");
  printf("%.2f\t%.2f\t%.2f\n\n", close, high, low);

  printf("S3: %.4f %.4f\nS2: %.4f %.4f\nS1: %.4f %.4f\nPP: %.4f %.4f\nR1: %.4f %.4f\nR2: %.4f %.4f\nR3: %.4f %.4f\n",
	 cl_s3, fi_s3, cl_s2, fi_s2, cl_s1, fi_s1, cl_pp, fi_pp, cl_r1, fi_r1, cl_r2, fi_r2, cl_r3, fi_r3);

  return 0;
}
