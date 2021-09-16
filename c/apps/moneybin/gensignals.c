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

static void print_movavg(ta_bars_t *b, double *ma1, double *ma2)
{
  int i;
  int year, month, day;
  timestamp_t *timestamp;

  printf("# date\t open\t high\t low\t close\t adjClose\t volume\t 5-day moving average close\t 20-day moving average close\n");

  for (i = 0; i < b->numrows; i++) {
    timestamp = &b->timestamp[i];

    ta_getdate(timestamp, &year, &month, &day);

    printf("%.4d-%.2d-%.2d ", year, month, day);
    printf("%9.3f ", b->open[i]);
    printf("%9.3f ", b->high[i]);
    printf("%9.3f ", b->low[i]);
    printf("%9.3f ", b->close[i]);
    printf("%9.3f ", b->adjclose[i]);
    printf("%d ", b->volume[i]);
    printf("%9.3f ", *(ma1 + i));
    printf("%9.3f", *(ma2 + i));
    putc('\n', stdout);
  }
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *ma1, *ma2;
  int period1 = 5;  /* fast period */
  int period2 = 20; /* slow period */
  int year, month, day;
  timestamp_t *timestamp;
  size_t pos;
  int crossover;
  int tradeno; /* number of trades */
  int ordercnt;

  if (argc < 2) {
    fprintf(stderr, "usage: gensignals <filename>\n");
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
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Allocate memory for the moving average array. */
  ma2 = malloc(sizeof(double) * bars->numrows);
  if (ma2 == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Calculate the moving averages. */
  sma(bars->close, bars->numrows, period1, ma1);
  sma(bars->close, bars->numrows, period2, ma2);

  print_array(bars->close, bars->numrows);
  print_array(ma1, bars->numrows);
  print_array(ma2, bars->numrows);

  print_movavg(bars, ma1, ma2);

  tradeno = 0;
  ordercnt = 0;

  printf("Trade\t%-10s\t%-7s\t%-6s\t%-8s\n", "Date", "Signal", "Close", "SMA (5)");

  for (pos = 0; pos < bars->numrows; pos++) {
    if (pos < period2) {
      continue;
    }

    crossover = ta_crossover(ma1, ma2, pos);

    timestamp = &bars->timestamp[pos];
    ta_getdate(timestamp, &year, &month, &day);

    if (crossover == TA_UP) {
      tradeno++;
      ordercnt++;
      printf("%d\t%.4d-%.2d-%.2d\tBUY\t%-5.2f\t%-7.2f\n", tradeno, year, month, day, *(bars->close + pos), *(ma1 + pos));
    } else if (crossover == TA_DOWN) {
      tradeno++;
      ordercnt++;
      printf("%d\t%.4d-%.2d-%.2d\tSELL\t%-5.2f\t%-7.2f\n", tradeno, year, month, day, *(bars->close + pos), *(ma1 + pos));
    }
  }

  printf("%d orders\n", ordercnt);

  /* Free allocated memory. */
  free(ma1);
  free(ma2);

  return 0;
}
