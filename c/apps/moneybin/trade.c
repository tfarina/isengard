#include <stdlib.h>
#include <stdio.h>

#include "crossover.h"
#include "sma.h"
#include "ta.h"

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
  int err;
  char filename[13] = "PETR4.SA.csv";
  ta_bars_t *bars;
  double *ma1, *ma2;

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for the moving average array. */
  ma1 = malloc(sizeof(double) * bars->numrows);
  if (ma1 == NULL) {
    return -1;
  }

  /* Allocate memory for the moving average array. */
  ma2 = malloc(sizeof(double) * bars->numrows);
  if (ma2 == NULL) {
    return -1;
  }

  /* Calculate the moving averages. */
  smovavg(bars->close, bars->numrows, 5, ma1);
  smovavg(bars->close, bars->numrows, 20, ma2);


  print_array(bars->close, bars->numrows);
  print_array(ma1, bars->numrows);
  print_array(ma2, bars->numrows);

  return 0;
}
