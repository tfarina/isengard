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
  double arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  size_t size = ARRAY_SIZE(arr);
  int period = 4;
  double *res;

  res = malloc(sizeof(double) * size);
  if (res == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  sma(arr, size, period, res);

  print_array(arr, size);
  print_array(res, size);

  free(res);

  return 0;
}
