#include <math.h>
#include <stdio.h>

/**
 * @param n Represents the number of periods (i.e., 10 years).
 */
static double future_value(double present_value, double interest_rate, double n)
{
  return present_value * pow(1 + interest_rate, n);
}

int main(int argc, char **argv)
{
  double fv;

  fv = future_value(2.20, 0.0972, 10);

  printf("Future Value: $%.2f\n", fv);

  return 0;
}
