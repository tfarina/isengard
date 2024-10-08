#include <stdio.h>

#include "arraysize.h"
#include "stats.h"

static void print_array(double const *arr, size_t const size)
{
  size_t i;

  fputc('[', stdout);

  for (i = 0; i < size; i++) {
    if (i) {
      fputs(", ", stdout);
    }
    printf("%.02f", *(arr + i));
  }

  fputc(']', stdout);
}

int main(void)
{
  double data[] = {1.6, 2.7, 3.8, 4.9};
  double min = 0.0;
  double max = 0.0;
  double sum = 0.0;
  double avg = 0.0;
  double variance = 0.0;
  double stdev = 0.0;

  printf("Array: ");
  print_array(data, ARRAY_SIZE(data));

  min = stats_min(data, ARRAY_SIZE(data));
  max = stats_max(data, ARRAY_SIZE(data));
  sum = stats_sum(data, ARRAY_SIZE(data));
  avg = stats_avg(data, ARRAY_SIZE(data));
  variance = stats_var(data, ARRAY_SIZE(data));
  stdev = stats_stdev(data, ARRAY_SIZE(data));

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
