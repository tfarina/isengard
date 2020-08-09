#include "stats.h"

#include <math.h>

int stats_min(double const *a, int size, double *res)
{
  double min;
  int i;

  if (size <= 0) {
    return -1;
  }

  min = a[0];

  for (i = 1; i < size; i++) {
    if (a[i] < min) {
      min = a[i];
    }
  }

  *res = min;

  return 0;
}

int stats_max(double const *a, int size, double *res)
{
  double max;
  int i;

  if (size <= 0) {
    return -1;
  }

  max = a[0];

  for (i = 1; i < size; i++) {
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

int stats_avg(double const *a, int size, double *res)
{
  double sum = 0.0;

  stats_sum(a, size, &sum);

  *res = sum / size;

  return 0;
}

int stats_var(double const *a, int size, double *res)
{
  double avg = 0.0;
  double sum = 0.0;
  int i;

  stats_avg(a, size, &avg);

  for (i = 0; i < size; i++) {
    sum += (a[i] - avg) * (a[i] - avg);
  }

  *res = sum / (size - 1);

  return 0;
}

int stats_stdev(double const *a, int size, double *res)
{
  double var = 0.0;

  stats_var(a, size, &var);
  *res = sqrt(var);

  return 0;
}
