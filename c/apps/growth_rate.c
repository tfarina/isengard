#include <math.h>
#include <stdio.h>

/**
 * Compounding Annual Growth Rate (or CAGR).
 *
 * @param n Represents the number of periods (i.e., 10 years).
 */
static double growth_rate(double future_value, double present_value, double n)
{
  return (pow(future_value / present_value, 1/n) - 1);
}

int main(int argc, char **argv)
{
  double gr;

  gr = growth_rate(310, 205, 10);

  printf("Compound Annual Growth Rate: %.2f%%\n", gr * 100);

  return 0;
}
