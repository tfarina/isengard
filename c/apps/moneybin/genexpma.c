#include <stdlib.h>
#include <stdio.h>

#include "ema.h"
#include "ta.h"

static void print_movavg(ta_bars_t *b, double *ma1, double *ma2) {
  int i;
  int year, month, day;
  timestamp_t *timestamp;

  printf("# date\t open\t high\t low\t close\t adjClose\t volume\t 12-day moving average close\t 26-day moving average close\n");

  for (i = 0; i < b->numrows; i++) {
    timestamp = &b->timestamp[i];

    ta_getdate(timestamp, &year, &month, &day);

    printf("%04d-%02d-%02d ", year, month, day);
    printf("%9.3f ", b->open[i]);
    printf("%9.3f ", b->high[i]);
    printf("%9.3f ", b->low[i]);
    printf("%9.3f ", b->close[i]);
    printf("%9.3f ", b->adjclose[i]);
    printf("%d ", b->volume[i]);
    printf("%9.3f ", *(ma1 + i));
    printf("%9.3f ", *(ma2 + i));
    putc('\n', stdout);
  }
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *ma1, *ma2;
  int period1 = 12; /* fast period */
  int period2 = 26; /* slow period */

  if (argc < 2) {
    fprintf(stderr, "usage: genexpma <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   genexpma PETR4.SA.csv > gnuplot/data/petr4_ema.dat\n", stdout);
    return 1;
  }

  filename = argv[1];

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
  ema(bars->close, bars->numrows, period1, ma1);
  ema(bars->close, bars->numrows, period2, ma2);

  print_movavg(bars, ma1, ma2);

  return 0;
}
