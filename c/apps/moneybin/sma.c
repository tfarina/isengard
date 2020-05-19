#include <stdio.h>

#include "arraysize.h"

static void print_array(double const *arr, size_t const size)
{
  size_t i;

  for (i = 0; i < size; i++) {
    if (i) {
      printf(", ");
    }
    printf("%.2f", *(arr + i));
  }

  printf("\n\n");
}

int ta_sma(int size, double const *const *inputs,
           double const *options, double *const *outputs) {
  return 0;
}

int main(void) {
  double bars_in[] = { 0.61, 0.60, 0.63, 0.65, 0.69, 0.67, 0.70, 0.68, 0.62, 0.63, 0.61 };
  int bars_size = ARRAY_SIZE(bars_in);

  print_array(bars_in, bars_size);

  return 0;
}

