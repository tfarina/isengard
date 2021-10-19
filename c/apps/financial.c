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
  double value_ratio = 0;
  double time_ratio = 0;
  double result = 0;

  value_ratio = future_value / present_value;
  time_ratio = 1 / n;

  result = pow(value_ratio, time_ratio);

  return result - 1;
}

double current_ratio(double current_assets, double current_liabilities)
{
  double result = 0;

  result = current_assets / current_liabilities;

  return result;
}
