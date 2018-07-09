#include "stats.h"

#include <math.h>

double stats_sum(double a[], size_t size)
{
  double sum = 0.0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += a[i];
  }

  return sum;
}

double stats_mean(double a[], size_t size)
{
  double sum = 0.0;

  sum = stats_sum(a, size);

  return sum / size;
}

double stats_var(double a[], size_t size)
{
  double avg = 0.0;
  double sum = 0.0;
  size_t i;

  avg = stats_mean(a, size);

  for (i = 0; i < size; i++) {
    sum += (a[i] - avg) * (a[i] - avg);
  }

  return sum / (size - 1);
}

double stats_stdev(double a[], size_t size)
{
  return sqrt(stats_var(a, size));
}
