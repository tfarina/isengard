#include "rsi.h"

#include <stdlib.h>
#include <stdio.h>

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

#if 0
  printf("Sum: %.2f\n", sum);
  printf("Up Average: %.2f\n", a2[i - 1]);
#endif

  for (; i < size; i++) {
    if (a1[i] > a1[i - 1]) {
      sum = a1[i] - a1[i - 1];
    } else {
      sum = 0;
    }
    a2[i] = ((a2[i - 1] * (period - 1)) + sum) / period;
  }

#if 0
  printf("Up Average: %.2f\n", a2[i - 1]);
#endif
}

void down_average(double const *a1, size_t size, int period, double *a2)
{
  int i;
  double sum = 0;

  a2[0] = 0;

  for (i = 1; i <= period && i < size; i++) {
    if (a1[i] < a1[i - 1]) {
      sum += a1[i - 1] - a1[i];
#if 0
      printf("i: %d Sum: %.2f\n", i, sum);
#endif
    }
    a2[i] = 0;
  }

  a2[i - 1] = sum / period;

#if 0
  printf("Down Average: %.2f\n", a2[i - 1]);
#endif

  for (; i < size; i++) {
    if (a1[i] < a1[i - 1]) {
      sum = a1[i - 1] - a1[i];
    } else {
      sum = 0;
    }
    a2[i] = ((a2[i - 1] * (period - 1)) + sum) / period;
  }

#if 0
  printf("Down Average: %.2f\n", a2[i - 1]);
#endif
}

int ind_rsi(double const *input, size_t size, int period, double *output)
{
  double *gains;
  double *losses;
  int i;

  /* Allocate memory for Gains array. */
  gains = malloc(sizeof(double) * size);
  if (gains == NULL) {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  /* Allocate memory for Losses array. */
  losses = malloc(sizeof(double) * size);
  if (losses == NULL) {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  up_average(input, size, period, gains);
  down_average(input, size, period, losses);

  for (i = period; i < size; i++) {
    double rs = gains[i] / losses[i];
#if 0
    printf("RS: %.2f\n", rs);
#endif

    output[i] = 100 - (100 / (1 + rs));

#if 0
    printf("RSI: %.2f\n", output[i]);
#endif
  }

  free(gains);
  free(losses);

  return 0;
}
