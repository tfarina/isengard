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

int main(void)
{
  double a[] = {1, 2, 3};
  double sum = 0.0;
  double mean = 0.0;

  sum = stats_sum(a, ARRAY_SIZE(a));
  mean = stats_mean(a, ARRAY_SIZE(a));

  printf("%f\n", sum);
  printf("%f\n", mean);

  return 0;
}
