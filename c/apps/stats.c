#include "stats.h"

#include <math.h>

int stats_min(double const *data, size_t size, double *res)
{
  double min;
  size_t i;

  if (size == 0) {
    return -1;
  }

  min = data[0];

  for (i = 1; i < size; i++) {
    if (data[i] < min) {
      min = data[i];
    }
  }

  *res = min;

  return 0;
}

int stats_max(double const *data, size_t size, double *res)
{
  double max;
  size_t i;

  if (size == 0) {
    return -1;
  }

  max = data[0];

  for (i = 1; i < size; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }

  *res = max;

  return 0;
}

int stats_sum(double const *data, size_t size, double *res)
{
  double sum = 0.0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += data[i];
  }

  *res = sum;

  return 0;
}

int stats_avg(double const *data, size_t size, double *res)
{
  double sum = 0.0;

  stats_sum(data, size, &sum);

  *res = sum / size;

  return 0;
}

int stats_var(double const *data, size_t size, double *res)
{
  double avg = 0.0;
  double sum = 0.0;
  size_t i;

  stats_avg(data, size, &avg);

  for (i = 0; i < size; i++) {
    sum += (data[i] - avg) * (data[i] - avg);
  }

  *res = sum / (size - 1);

  return 0;
}

int stats_stdev(double const *data, size_t size, double *res)
{
  double var = 0.0;

  stats_var(data, size, &var);
  *res = sqrt(var);

  return 0;
}
