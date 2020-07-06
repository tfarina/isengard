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

double future_value(double pv, double i, double n)
{
  return pv * pow(1 + i, n);
}

double present_value(double fv, double i, double n)
{
  return fv / pow(1 + i, n);
}

/**
 * Compounding Annual Growth Rate (or CAGR).
 *
 * @param n the number of periods (e.g., 10 years).
 */
double growth_rate(double future_value, double present_value, double n)
{
  return (pow(future_value / present_value, 1/n) - 1);
}

