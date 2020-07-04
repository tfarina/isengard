#include <stdlib.h>

#include "crossover.h"
#include "ta.h"
#include "vector.h"

int main(void)
{
  int err;
  char filename[13] = "PETR4.SA.csv";
  ta_bars_t *bars;
  vector_t /* of double */ *ma1, *ma2;

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for the moving average array. */
  ma1 = vector_alloc(sizeof(double), bars->numrows);
  if (ma1 == NULL) {
    return -1;
  }

  /* Allocate memory for the moving average array. */
  ma2 = vector_alloc(sizeof(double), bars->numrows);
  if (ma2 == NULL) {
    return -1;
  }

  return 0;
}
