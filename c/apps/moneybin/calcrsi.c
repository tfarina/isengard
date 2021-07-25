#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

void up_average(double const *a1, size_t size, int period, double *a2)
{
  int i;
  double sum = 0;

  a2[0] = 0;

  for (i = 1; i <= period && i < size; i++) {
    if (a1[i] > a1[i - 1]) {
      sum += a1[i] - a1[i - 1];
    }
    a2[i] = 0;
  }

  a2[i - 1] = sum / period;

  printf("Sum: %.2f\n", sum);
  printf("Up Average: %.2f\n", a2[i - 1]);
}

void down_average(double const *a1, size_t size, int period, double *a2)
{
  int i;
  double sum = 0;

  a2[0] = 0;

  for (i = 1; i <= period && i < size; i++) {
    if (a1[i] < a1[i - 1]) {
      sum += a1[i - 1] - a1[i];
      printf("i: %d Sum: %.2f\n", i, sum);
    }
    a2[i] = 0;
  }

  a2[i - 1] = sum / period;


  printf("Down Average: %.2f\n", a2[i - 1]);
}

void ind_rsi(double const *close, size_t size, int period, double *rsi)
{
  double upavg = 0, downavg = 0;
  int i;

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

  rsi[period] = 100 - (100 / (1 + rs));

  printf("RSI: %.2f\n", rsi[period]);
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *close;
  double *gains;
  double *losses;
  double *rsi;

  int period = 14;

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

  /* Allocate memory for RSI array. */
  rsi = malloc(sizeof(double) * bars->numrows);
  if (rsi == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Allocate memory for Gains array. */
  gains = malloc(sizeof(double) * bars->numrows);
  if (gains == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Allocate memory for Losses array. */
  losses = malloc(sizeof(double) * bars->numrows);
  if (losses == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  up_average(close, bars->numrows, period, gains);
  down_average(close, bars->numrows, period, losses);

  ind_rsi(close, bars->numrows, period, rsi);

  return 0;
}
