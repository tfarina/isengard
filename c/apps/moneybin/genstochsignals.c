#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ta.h"

#define ORDERCOLSTR "Order"
static int ordercolw;

#define OVERSOLD_LEVEL 20
#define OVERBOUGHT_LEVEL 80

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
  double *stoch;
  int period = 14;
  int year, month, day;
  timestamp_t *timestamp;
  size_t pos;
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

  /* Allocate memory for Stochastic array. */
  stoch = malloc(sizeof(double) * bars->numrows);
  if (stoch == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Calculate the Stochastic. */
  ind_stochastic(bars->high, bars->low, bars->close, bars->numrows, period, stoch);

#if 0
  print_array(bars->close, bars->numrows);
  print_array(stoch, bars->numrows);
#endif

  tradeno = 0;
  ordercnt = 0;
  intrade = 0;
  profitable = 0;
  pctprofit = 0;

  ordercolw = strlen(ORDERCOLSTR);

  printf("Stochastic System\n");
  putc('\n', stdout);
  printf("%*s %10s %s %9s %s\n", ordercolw, ORDERCOLSTR, "Date", "Signal", "Close", "P/L");

  for (pos = 0; pos < bars->numrows; pos++) {
    timestamp = &bars->timestamp[pos];
    ta_getdate(timestamp, &year, &month, &day);

    /*
     * First we have to evaluate the buy or sell signal in order to open the
     * trade.
     */
    if ((int)stoch[pos] < OVERSOLD_LEVEL && !intrade) {
      /*
       * If it crossed up and the current position is not Long, we have to
       * Buy to Open the trade.
       */
      tradeno++;
      ordercnt++;

      buyprice = *(bars->close + pos);

      printf("%-*d %.4d-%.2d-%.2d %6s %9.2f\n", ordercolw, ordercnt, year, month, day, "BUY", buyprice);

      intrade = 1;
    } else if ((int)stoch[pos] > OVERBOUGHT_LEVEL && intrade) {
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
  free(stoch);

  return 0;
}
