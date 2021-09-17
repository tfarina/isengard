#include <stdio.h>
#include <stdlib.h>

#include "arraysize.h"
#include "sma.h"

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

int main(void)
{
  double input[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  size_t input_size = ARRAY_SIZE(input);
  int period = 4;
  double *res;

  res = malloc(sizeof(double) * input_size);
  if (res == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  sma(input, input_size, period, res);

  print_array(input, input_size);
  print_array(res, input_size);

  free(res);

  return 0;
}
