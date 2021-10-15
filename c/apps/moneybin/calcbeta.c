#include <stdlib.h>
#include <stdio.h>

#include "beta.h"
#include "finance.h"
#include "stats.h"
#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *stock_filename;
  char *index_filename;
  ta_bars_t *stock_bars;
  ta_bars_t *index_bars;
  double *stock_daily_ret;
  double *index_daily_ret;
  double beta;
  int i;

  if (argc < 2) {
    fputs("usage: calcbeta <filename1> <filename2>\n", stderr);
    fputc('\n', stderr);
    fputs("Example:\n", stderr);
    fputc('\n', stderr);
    fputs("   calcbeta TWTR.csv ^GSPC.csv\n", stderr);
    return 1;
  }

  stock_filename = argv[1];
  index_filename = argv[2];

  /* Read bar data from CSV file. */
  err = read_csv(stock_filename, &stock_bars);
  if (err < 0) {
    return err;
  }

  /* Read bar data from CSV file. */
  err = read_csv(index_filename, &index_bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for stock daily returns array. */
  stock_daily_ret = malloc(sizeof(double) * stock_bars->numrows);
  if (stock_daily_ret == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Allocate memory for index daily returns array. */
  index_daily_ret = malloc(sizeof(double) * index_bars->numrows);
  if (index_daily_ret == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  daily_returns(stock_bars->close, stock_bars->numrows, stock_daily_ret);
  daily_returns(index_bars->close, index_bars->numrows, index_daily_ret);

  beta = beta_ratio(stock_daily_ret, stock_bars->numrows, index_daily_ret, index_bars->numrows);

  /* Beta (1Y daily) */
  printf("%6.2f\n", beta);

  /* Free memory allocated for stock and index daily return arrays. */
  free(stock_daily_ret);
  free(index_daily_ret);

  return 0;
}
