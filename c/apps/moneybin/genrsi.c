#include <stdlib.h>
#include <stdio.h>

#include "rsi.h"
#include "ta.h"

static void print_rsi(ta_bars_t *b, double const *rsi, int period)
{
  int i;
  int year, month, day;
  timestamp_t *timestamp;

  printf("# date\t open\t high\t low\t close\t adjClose\t volume\t RSI\n");

  for (i = period; i < b->numrows; i++) {
    timestamp = &b->timestamp[i];

    ta_getdate(timestamp, &year, &month, &day);

    printf("%04d-%02d-%02d ", year, month, day);
    printf("%9.3f ", b->open[i]);
    printf("%9.3f ", b->high[i]);
    printf("%9.3f ", b->low[i]);
    printf("%9.3f ", b->close[i]);
    printf("%9.3f ", b->adjclose[i]);
    printf("%d ", b->volume[i]);
    printf("%9.3f ", *(rsi + i));
    putc('\n', stdout);
  }
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *rsi;
  int period = 14;

  if (argc < 2) {
    fprintf(stderr, "usage: genrsi <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   genrsi PETR4.SA.csv > gnuplot/data/petr4_rsi.dat\n", stdout);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for RSI array. */
  rsi = malloc(sizeof(double) * bars->numrows);
  if (rsi == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Calculate RSI. */
  ind_rsi(bars->close, bars->numrows, period, rsi);

  print_rsi(bars, rsi, period);

  return 0;
}
