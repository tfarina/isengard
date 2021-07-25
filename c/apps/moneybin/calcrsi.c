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

  for (; i < size; i++) {
    if (a1[i] > a1[i - 1]) {
      sum = a1[i] - a1[i - 1];
    } else {
      sum = 0;
    }
    a2[i] = ((a2[i - 1] * (period - 1)) + sum) / period;
  }
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

  for (; i < size; i++) {
    if (a1[i] < a1[i - 1]) {
      sum = a1[i - 1] - a1[i];
    } else {
      sum = 0;
    }
    a2[i] = ((a2[i - 1] * (period - 1)) + sum) / period;
  }
  printf("Down Average: %.2f\n", a2[i - 1]);
}

void ind_rsi(double const *close, size_t size, int period, double *rsi)
{
  double *gains;
  double *losses;
  int i;

  /* Allocate memory for Gains array. */
  gains = malloc(sizeof(double) * size);
  if (gains == NULL) {
    fprintf(stderr, "Out of memory\n");
    return;
  }

  /* Allocate memory for Losses array. */
  losses = malloc(sizeof(double) * size);
  if (losses == NULL) {
    fprintf(stderr, "Out of memory\n");
    return;
  }

  up_average(close, size, period, gains);
  down_average(close, size, period, losses);

  for (i = period; i < size; i++) {
    double rs = gains[i] / losses[i];
    printf("RS: %.2f\n", rs);

    rsi[i] = 100 - (100 / (1 + rs));
    printf("RSI: %.2f\n", rsi[i]);
  }

  free(gains);
  free(losses);
}

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *close;
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

  ind_rsi(close, bars->numrows, period, rsi);

  return 0;
}
