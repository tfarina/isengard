#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ta.h"

#define ORDERCOLSTR "Order"
static int ordercolw;

#define OVERSOLD_LEVEL 30
#define OVERBOUGHT_LEVEL 70

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

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *rsi;
  int period = 14;
  int year, month, day;
  timestamp_t *timestamp;
  size_t pos;
  int signal;
  int tradeno; /* number of trades */
  int ordercnt;
  int intrade;
  double buyprice;
  double sellprice;
  double pl; /* profit or loss in the trade */
  float profitable; /* number of profitable trades */
  float pctprofit; /* percentage of profitable trades */

  if (argc < 2) {
    fprintf(stderr, "usage: genrsisignals <filename>\n");
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

  /* Calculate the RSI. */
  ind_rsi(bars->close, bars->numrows, period, rsi);

#if 0
  print_array(bars->close, bars->numrows);
  print_array(rsi, bars->numrows);
#endif

  tradeno = 0;
  ordercnt = 0;
  intrade = 0;
  profitable = 0;
  pctprofit = 0;

  ordercolw = strlen(ORDERCOLSTR);

  printf("RSI System\n");
  putc('\n', stdout);
  printf("%*s %10s %s %9s %s\n", ordercolw, ORDERCOLSTR, "Date", "Signal", "Close", "P/L");

  for (pos = 0; pos < bars->numrows; pos++) {
    signal = ta_strat_rsi(rsi[pos], OVERSOLD_LEVEL, OVERBOUGHT_LEVEL);

    timestamp = &bars->timestamp[pos];
    ta_getdate(timestamp, &year, &month, &day);

    /*
     * First, evaluate the BUY or SELL signal.
     */
    if (signal == TA_SIGNAL_BUY && !intrade) {
      ordercnt++;

      /*
       * If it crossed up and the current position is not Long, we have to
       * Buy to Open the trade.
       */
      tradeno++;
      buyprice = *(bars->close + pos);

      printf("%-*d %.4d-%.2d-%.2d %6s %9.2f\n", ordercolw, ordercnt, year, month, day, "BUY", buyprice);

      intrade = 1;
    } else if (signal == TA_SIGNAL_SELL && intrade) {
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
  free(rsi);

  return 0;
}
