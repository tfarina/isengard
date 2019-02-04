#include <math.h>
#include <stdio.h>

/**
 * Computes the future value.
 *
 * @param pv   a present value
 * @param rate an interest 'rate' compounded once per period
 * @param n    the number of periods (e.g., 10 years)
 *
 * @return the value at the end of 'n' periods
 */
static double future_value(double pv, double rate, double n)
{
  return pv * pow(1 + rate, n);
}

int main(int argc, char **argv)
{
  double fv;

  fv = future_value(2.20, 0.0972, 10);

  printf("Future Value: $%.2f\n", fv);

  return 0;
}
