#include <stdio.h>

#include "stats.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

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

double stats_variance(double a[], size_t size)
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

int main(void)
{
  double a[] = {1.6, 2.7, 3.8, 4.9};
  double sum = 0.0;
  double mean = 0.0;
  double variance = 0.0;

  sum = stats_sum(a, ARRAY_SIZE(a));
  mean = stats_mean(a, ARRAY_SIZE(a));
  variance = stats_variance(a, ARRAY_SIZE(a));

  printf("%f\n", sum);
  printf("%f\n", mean);
  printf("%f\n", variance);

  return 0;
}
