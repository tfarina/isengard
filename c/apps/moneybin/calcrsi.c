#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *close;
  double *res;

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

  close = bars->close;

  res = malloc(sizeof(double) * bars->numrows);
  if (res == NULL) {
    return -1;
  }

  double upavg, downavg;
  int i;
  int period = 14;
  
  for (i = 1; i <= period; i++) {
    double upward = close[i] > close[i-1] ? close[i] - close[i-1] : 0;
    double downward = close[i] < close[i-1] ? close[i-1] - close[i] : 0;

    upavg += upward;
    downavg += downward;
  }
  printf("Gain: %.2f\n", upavg);
  printf("Loss: %.2f\n", downavg);

  upavg /= period;
  downavg /= period;

  printf("Avg Gain: %.2f\n", upavg);
  printf("Avg Loss: %.2f\n", downavg);

  double rs = upavg / downavg;
  printf("RS: %.2f\n", rs);

  /* Allocate memory for RSI array. */
  double *rsi;

  rsi = malloc(sizeof(double) * bars->numrows);

  rsi[period] = 100 - (100 / (1 + rs));

  printf("RSI: %.2f\n", rsi[period]);

  return 0;
}
