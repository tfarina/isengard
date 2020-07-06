#include <stdio.h>

#include "arraysize.h"
#include "stats.h"

static void print_array(double const *arr, size_t const size)
{
  size_t i;

  for (i = 0; i < size; i++) {
    if (i) {
      printf(", ");
    }
    printf("%.02f", *(arr + i));
  }

  printf("\n\n");
}

int main(void)
{
  double a[] = {1.6, 2.7, 3.8, 4.9};
  double sum = 0.0;
  double avg = 0.0;
  double variance = 0.0;
  double stdev = 0.0;
  double min = 0.0;
  double max = 0.0;

  printf("Array: ");
  print_array(a, ARRAY_SIZE(a));

  stats_sum(a, ARRAY_SIZE(a), &sum);
  stats_average(a, ARRAY_SIZE(a), &avg);
  stats_var_sam(a, ARRAY_SIZE(a), &variance);
  stats_stdev_sam(a, ARRAY_SIZE(a), &stdev);
  stats_min(a, ARRAY_SIZE(a), &min);
  stats_max(a, ARRAY_SIZE(a), &max);

  printf("Statistics summary:\n");
  printf("Sum                 %f\n", sum);
  printf("Average             %f\n", avg);
  printf("Variance            %f\n", variance);
  printf("Standard deviation  %f\n", stdev);
  printf("Min                 %f\n", min);
  printf("Max                 %f\n", max);

  return 0;
}
