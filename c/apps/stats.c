#include "stats.h"

#include <math.h>

double stats_min(double *a, size_t size)
{
  size_t i;
  double min = +INFINITY;

  for (i = 0; i < size; i++) {
    if(isnan(a[i])) {
      return NAN;
    }

    if (a[i] < min) {
      min = a[i];
    }
  }

  return min;
}

double stats_max(double *a, size_t size)
{
  size_t i;
  double max = -INFINITY;

  for (i = 0; i < size; i++) {
    if(isnan(a[i])) {
      return NAN;
    }

    if (a[i] > max) {
      max = a[i];
    }
  }

  return max;
}

int stats_sum(double *a, size_t size, double *res)
{
  double sum = 0.0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += a[i];
  }

  *res = sum;

  return 0;
}

double stats_average(double *a, size_t size)
{
  double sum = 0.0;

  stats_sum(a, size, &sum);

  return sum / size;
}

double stats_var(double *a, size_t size)
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

double stats_stdev(double *a, size_t size)
{
  return sqrt(stats_var(a, size));
}
