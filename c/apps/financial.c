#include "financial.h"

#include <math.h>

double future_value(double pv, double i, double n)
{
  double cf = pow(1 + i, n);

  return pv * cf;
}

double present_value(double fv, double i, double n)
{
  double df = pow(1 + i, n);

  return fv / df;
}

double growth_rate(double future_value, double present_value, double n)
{
  return (pow(future_value / present_value, 1/n) - 1);
}
