#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crossover.h"
#include "sma.h"
#include "ta.h"

#define ORDERCOLSTR "Order"
static int ordercolw;

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
  int intrade;
  double buyprice;
  double sellprice;
  double pl; /* profit or loss in the trade */
  float profitable; /* number of profitable trades */
  float pctprofit; /* percentage of profitable trades */

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
  ind_sma(bars->close, bars->numrows, period1, ma1);
  ind_sma(bars->close, bars->numrows, period2, ma2);

#if 0
  print_array(bars->close, bars->numrows);
  print_array(ma1, bars->numrows);
  print_array(ma2, bars->numrows);

  print_movavg(bars, ma1, ma2);
#endif

  tradeno = 0;
  ordercnt = 0;
  intrade = 0;
  profitable = 0;
  pctprofit = 0;

  ordercolw = strlen(ORDERCOLSTR);

  printf("SMA Crossover System\n");
  putc('\n', stdout);
  printf("%*s %10s %s %9s %s\n", ordercolw, ORDERCOLSTR, "Date", "Signal", "Close", "P/L");

  for (pos = 0; pos < bars->numrows; pos++) {
    if (pos < period2) {
      continue;
    }

    crossover = ta_crossover(ma1, ma2, pos);

    timestamp = &bars->timestamp[pos];
    ta_getdate(timestamp, &year, &month, &day);

    /*
     * First we have to evaluate the buy or sell signal in order to open the
     * trade.
     */
    if (crossover == TA_UP && !intrade) {
      /*
       * If it crossed up and the current position is not Long, we have to
       * Buy to Open the trade.
       */
      tradeno++;
      ordercnt++;

      buyprice = *(bars->close + pos);

      printf("%-*d %.4d-%.2d-%.2d %6s %9.2f\n", ordercolw, ordercnt, year, month, day, "BUY", buyprice);

      intrade = 1;
    } else if (crossover == TA_DOWN && intrade) {
      ordercnt++;

      sellprice = *(bars->close + pos);

      pl = sellprice - buyprice;

      if (pl > 0) {
        profitable++;
      }

      printf("%-*d %.4d-%.2d-%.2d %6s %9.2f %-9.2f\n", ordercolw, ordercnt, year, month, day, "SELL", sellprice, pl);

      intrade = 0;
    }
  }

  putc('\n', stdout);

  pctprofit = (profitable / tradeno) * 100;

  printf("total trades: %d\n", tradeno);
  printf("profitable trades: %0.f\n", profitable);
  printf("percentage of profitable trades: %0.2f%%\n", pctprofit);

  /* Free allocated memory. */
  free(ma1);
  free(ma2);

  return 0;
}
