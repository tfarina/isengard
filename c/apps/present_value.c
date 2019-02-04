#include <math.h>
#include <stdio.h>

/**
 * Computes the present value.
 *
 * @param fv   a future value
 * @param rate an interest 'rate' compounded once per period
 * @param n    the number of periods (e.g., 10 years).
 */
static double present_value(double fv, double rate, double n)
{
  return fv / pow(1 + rate, n);
}

int main(int argc, char **argv)
{
  double pv;

  pv = present_value(2.20, 0.0972, 10);

  printf("Present Value: $%.2f\n", pv);

  return 0;
}
