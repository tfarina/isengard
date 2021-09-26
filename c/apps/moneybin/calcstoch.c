#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *stoch;
  int period = 14;
  int i;

  if (argc < 2) {
    fprintf(stderr, "usage: calcstoch <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   calcstoch BANCOPAN.csv\n", stdout);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for Stochastic array. */
  stoch = malloc(sizeof(double) * bars->numrows);
  if (stoch == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  ind_stochastic(bars->high, bars->low, bars->close, bars->numrows, period, stoch);

  return 0;
}
