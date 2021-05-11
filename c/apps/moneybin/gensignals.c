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
  int window1 = 5 /* fast period */, window2 = 20 /* slow period */;
  int year, month, day;
  timestamp_t *timestamp;
  size_t pos;
  int crossover;
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
    return -1;
  }

  /* Allocate memory for the moving average array. */
  ma2 = malloc(sizeof(double) * bars->numrows);
  if (ma2 == NULL) {
    return -1;
  }

  /* Calculate the moving averages. */
  smovavg(bars->close, bars->numrows, window1, ma1);
  smovavg(bars->close, bars->numrows, window2, ma2);


  print_array(bars->close, bars->numrows);
  print_array(ma1, bars->numrows);
  print_array(ma2, bars->numrows);

  print_movavg(bars, ma1, ma2);

  ordercnt = 0;

  printf("Date\tOrder Type\tSignal\tEnter\tClose\tSMA (5)\n");

  for (pos = 0; pos < bars->numrows; pos++) {
    if (pos < window2) {
      continue;
    }

    crossover = ta_crossover(ma1, ma2, pos);

    timestamp = &bars->timestamp[pos];
    ta_getdate(timestamp, &year, &month, &day);

    if (crossover == TA_UP) {
      ordercnt++;
      printf("%02d-%02d-%04d LIMIT BUY LONG %9.2f %9.2f\n", month, day, year, *(bars->close + pos), *(ma1 + pos));
    } else if (crossover == TA_DOWN) {
      ordercnt++;
      printf("%02d-%02d-%04d LIMIT SELL SHORT %9.2f %9.2f\n", month, day, year, *(bars->close + pos), *(ma1 + pos));
    }
  }

  printf("%d orders\n", ordercnt);

  return 0;
}
