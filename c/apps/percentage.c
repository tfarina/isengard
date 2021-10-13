#include "percentage.h"

double percentage_change(double old_value, double new_value)
{
  double diff = 0.0;

  diff = new_value - old_value;

  return (diff / old_value) * 100;
}
