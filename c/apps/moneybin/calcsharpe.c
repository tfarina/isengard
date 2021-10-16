#include <stdlib.h>
#include <stdio.h>

#include "finance.h"
#include "sharpe.h"
#include "stats.h"
#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *daily_ret;
  double avg_ret, stdev_ret, sharpe;
  double risk_free_rate = 0.00038;
  int i;

  if (argc < 2) {
    fputs("usage: calcsharpe <filename>\n", stderr);
    fputc('\n', stderr);
    fputs("Example:\n", stderr);
    fputc('\n', stderr);
    fputs("   calcsharpe TWTR.csv\n", stderr);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for daily returns array. */
  daily_ret = malloc(sizeof(double) * bars->numrows);
  if (daily_ret == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  daily_returns(bars->close, bars->numrows, daily_ret);
  avg_ret = stats_avg(daily_ret, bars->numrows);
  stdev_ret = stats_stdev(daily_ret, bars->numrows);

  sharpe = sharpe_ratio(avg_ret, stdev_ret, risk_free_rate);

  printf("%6.2f\n", sharpe);

  /* Free memory allocated for daily returns array. */
  free(daily_ret);

  return 0;
}
