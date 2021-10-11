#include "stats.h"

#include <math.h>

double stats_min(double const *data, size_t size)
{
  double min;
  size_t i;

  min = data[0];

  for (i = 1; i < size; i++) {
    if (data[i] < min) {
      min = data[i];
    }
  }

  return min;
}

double stats_max(double const *data, size_t size)
{
  double max;
  size_t i;

  max = data[0];

  for (i = 1; i < size; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }

  return max;
}

double stats_sum(double const *data, size_t size)
{
  double sum = 0;
  size_t i;

  for (i = 0; i < size; i++) {
    sum += data[i];
  }

  return sum;
}

double stats_avg(double const *data, size_t size)
{
  double sum = 0;

  sum = stats_sum(data, size);

  return sum / size;
}

double stats_var(double const *data, size_t size)
{
  double avg = 0;
  double sum = 0;
  size_t i;

  avg = stats_avg(data, size);

  for (i = 0; i < size; i++) {
    sum += (data[i] - avg) * (data[i] - avg);
  }

  return sum / (size - 1);
}

double stats_stdev(double const *data, size_t size)
{
  double var = 0;

  var = stats_var(data, size);

  return sqrt(var);
}
