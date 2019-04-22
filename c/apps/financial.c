#include "financial.h"

#include <math.h>

static double change(double old_value, double new_value)
{
  double diff = 0.0;

  diff = new_value - old_value;

  return diff / old_value;
}

double percentage_change(double old_value, double new_value)
{
  return change(old_value, new_value) * 100;
}

double future_value(double pv, double rate, double n)
{
  return pv * pow(1 + rate, n);
}

double present_value(double fv, double rate, double n)
{
  return fv / pow(1 + rate, n);
}

/**
 * Compounding Annual Growth Rate (or CAGR).
 *
 * @param n Represents the number of periods (i.e., 10 years).
 */
double growth_rate(double future_value, double present_value, double n)
{
  return (pow(future_value / present_value, 1/n) - 1);
}

