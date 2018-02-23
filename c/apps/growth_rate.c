#include <math.h>
#include <stdio.h>

/**
 * |n| represents the number of periods (i.e., 10 years).
 */
static double growth_rate(double present, double past, double n)
{
  return (pow((present / past), 1/n) - 1) * 100;
}

int main(int argc, char **argv)
{
  double gr;

  gr = growth_rate(310, 205, 10);

  printf("%.2f%%\n", gr);

  return 0;
}
