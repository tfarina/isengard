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
