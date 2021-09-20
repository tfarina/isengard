#include <stdio.h>

#include "financial.h"

int main(int argc, char **argv)
{
  double present_val, rate, num_periods;
  double fv;

  present_val = 2.20;
  rate = 0.0972; /* 9.72% */
  num_periods = 10;

  fv = future_value(present_val, rate, num_periods);

  printf("Future Value: $%.2f\n", fv);

  return 0;
}
