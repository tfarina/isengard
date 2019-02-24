#include "stats.h"

#include <math.h>

int stats_min(double const *a, int size, double *res)
{
  int i;
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

int stats_max(double const *a, int size, double *res)
{
  int i;
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

int stats_sum(double const *a, int size, double *res)
{
  double sum = 0.0;
  int i;

  for (i = 0; i < size; i++) {
    sum += a[i];
  }

  *res = sum;

  return 0;
}

int stats_average(double const *a, int size, double *res)
{
  double sum = 0.0;

  stats_sum(a, size, &sum);

  *res = sum / size;

  return 0;
}

int stats_var_sam(double const *a, int size, double *res)
{
  double avg = 0.0;
  double sum = 0.0;
  int i;

  stats_average(a, size, &avg);

  for (i = 0; i < size; i++) {
    sum += (a[i] - avg) * (a[i] - avg);
  }

  *res = sum / (size - 1);

  return 0;
}

int stats_stdev_sam(double const *a, int size, double *res)
{
  double var = 0.0;

  stats_var_sam(a, size, &var);
  *res = sqrt(var);

  return 0;
}
