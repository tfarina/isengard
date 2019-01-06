#include <math.h>
#include <stdio.h>

/**
 * @param n Represents the number of periods (i.e., 10 years).
 */
static double present_value(double future_value, double interest_rate, double n)
{
  return future_value / pow(1 + interest_rate, n);
}

int main(int argc, char **argv)
{
  double pv;

  pv = present_value(2.20, 0.0972, 10);

  printf("Present Value: $%.2f\n", pv);

  return 0;
}
