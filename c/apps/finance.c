#include "finance.h"

#include <math.h>

double future_value(double pv, double rate, double n)
{
  return pv * pow(1 + rate, n);
}

double present_value(double fv, double rate, double n)
{
  return fv / pow(1 + rate, n);
}
