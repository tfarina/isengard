#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

static void print_stoch(ta_bars_t *b, double const *stoch, int period)
{
  int i;
  int year, month, day;
  timestamp_t *timestamp;

  printf("# date\t open\t high\t low\t close\t adjClose\t volume\t STOCHASTIC\n");

  for (i = period; i < b->numrows; i++) {
    timestamp = &b->timestamp[i];

    ta_getdate(timestamp, &year, &month, &day);

    printf("%.4d-%.2d-%.2d ", year, month, day);
    printf("%9.3f ", b->open[i]);
    printf("%9.3f ", b->high[i]);
    printf("%9.3f ", b->low[i]);
    printf("%9.3f ", b->close[i]);
    printf("%9.3f ", b->adjclose[i]);
    printf("%d ", b->volume[i]);
    printf("%9.3f", *(stoch + i));
    putc('\n', stdout);
  }
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *stoch;
  int period = 14;

  if (argc < 2) {
    fprintf(stderr, "usage: genstoch <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   genstoch TWTR.csv > gnuplot/data/twtr_stoch.dat\n", stdout);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for STOCHASTIC array. */
  stoch = malloc(sizeof(double) * bars->numrows);
  if (stoch == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Calculate STOCHASTIC. */
  ind_stochastic(bars->high, bars->low, bars->close, bars->numrows, period, stoch);

  print_stoch(bars, stoch, period);

  /* Free allocated memory. */
  free(stoch);

  return 0;
}
