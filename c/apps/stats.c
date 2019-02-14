#include "stats.h"

#include <math.h>

int stats_min(double const *a, size_t size, double *res)
{
  size_t i;
  double min = +INFINITY;

  if (size <= 0) {
    return -1;
  }

  for (i = 0; i < size; i++) {
    if(isnan(a[i])) {
      return NAN;
    }

    if (a[i] < min) {
      min = a[i];
    }
  }

  *res = min;

  return 0;
}

int stats_max(double const *a, size_t size, double *res)
{
  size_t i;
  double max = -INFINITY;

  if (size <= 0) {
    return -1;
  }

  for (i = 0; i < size; i++) {
    if(isnan(a[i])) {
      return NAN;
    }

    if (a[i] > max) {
      max = a[i];
    }
  }

  *res = max;

  return 0;
}

int stats_sum(double const *a, size_t size, double *res)
{
  double sum = 0.0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += a[i];
  }

  *res = sum;

  return 0;
}

double stats_average(double const *a, size_t size)
{
  double sum = 0.0;

  stats_sum(a, size, &sum);

  return sum / size;
}

double stats_var(double const *a, size_t size)
{
  double avg = 0.0;
  double sum = 0.0;
  size_t i;

  avg = stats_average(a, size);

  for (i = 0; i < size; i++) {
    sum += (a[i] - avg) * (a[i] - avg);
  }

  return sum / (size - 1);
}

double stats_stdev(double const *a, size_t size)
{
  return sqrt(stats_var(a, size));
}
