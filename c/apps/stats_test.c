#include <stdio.h>

#include "arraysize.h"
#include "stats.h"

static void print_array(double const *arr, size_t const size)
{
  size_t i;

  putchar('[');

  for (i = 0; i < size; i++) {
    if (i) {
      fputs(", ", stdout);
    }
    printf("%.02f", *(arr + i));
  }

  putchar(']');
}

int main(void)
{
  double a[] = {1.6, 2.7, 3.8, 4.9};
  double min = 0.0;
  double max = 0.0;
  double sum = 0.0;
  double avg = 0.0;
  double variance = 0.0;
  double stdev = 0.0;

  printf("Array: ");
  print_array(a, ARRAY_SIZE(a));

  stats_min(a, ARRAY_SIZE(a), &min);
  stats_max(a, ARRAY_SIZE(a), &max);
  stats_sum(a, ARRAY_SIZE(a), &sum);
  stats_avg(a, ARRAY_SIZE(a), &avg);
  stats_var(a, ARRAY_SIZE(a), &variance);
  stats_stdev(a, ARRAY_SIZE(a), &stdev);

  printf("\n\n");
  printf("Statistics summary:\n");
  printf("Min                 %f\n", min);
  printf("Max                 %f\n", max);
  printf("Sum                 %f\n", sum);
  printf("Mean                %f\n", avg);
  printf("Variance            %f\n", variance);
  printf("Std. Dev.           %f\n", stdev);

  return 0;
}
