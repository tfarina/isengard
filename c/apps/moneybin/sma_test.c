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

/**
 * Calculates a simple moving average.
 *
 * Example:
 *
 * arr = [1, 2, 3, 4, 5, 6, 7, 8, 9]
 * outarr = [ '2.50', '3.50', '4.50', '5.50', '6.50', '7.50' ]
 *     //=>   │       │       │       │       │       └─(6+7+8+9)/4
 *     //=>   │       │       │       │       └─(5+6+7+8)/4
 *     //=>   │       │       │       └─(4+5+6+7)/4
 *     //=>   │       │       └─(3+4+5+6)/4
 *     //=>   │       └─(2+3+4+5)/4
 *     //=>   └─(1+2+3+4)/4
 */
static void smovavg(double const *arr, size_t size, int window, double *outarr)
{
  int i;
  double sum = 0;

  /* Calculate the average for each of the first elements. */
  for (i = 0; i < window; i++) {
    sum += *(arr + i);

    *(outarr + i) = sum / (i + 1);
  }

  for (i = window; i < size; i++) {
    sum -= *(arr + i - window);  /* Remove the old elements from the sum */

    sum += *(arr + i);  /* Add the new (or most recent) elements to the sum */


    /* Calculate the average */
    *(outarr + i) = sum / window;
  }
}

int main(void)
{
  double arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  size_t size = ARRAY_SIZE(arr);
  int window = 4;
  double *res;

  res = malloc(sizeof(double) * size);
  if (res == NULL) {
    return -1;
  }

  smovavg(arr, size, window, res);

  print_array(arr, size);
  print_array(res, size);

  free(res);

  return 0;
}
