#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *rsi;
  int period = 14;
  int i;

  if (argc < 2) {
    fprintf(stderr, "usage: calcrsi <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   calcrsi BANCOPAN.csv\n", stdout);
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

  /* Output values. */
  for (i = period; i < bars->numrows; i++) {
    printf("%f\n", *(rsi + i));
  }

  /* Free memory allocated for RSI array. */
  free(rsi);

  return 0;
}
