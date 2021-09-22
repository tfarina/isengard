#include "stochastic.h"

#include <stdio.h>

static int stats_range_min(double const *a, int size, int start, int stop, double *res)
{
  double min;
  int i;

  if (size <= 0) {
    return -1;
  }

  min = a[start];

  for (i = start; i <= stop && i < size; i++) {
    if (a[i] < min) {
      min = a[i];
    }
  }

  *res = min;

  return 0;
}

static int stats_range_max(double const *a, int size, int start, int stop, double *res)
{
  double max;
  int i;

  if (size <= 0) {
    return -1;
  }

  max = a[start];

  for (i = start; i <= stop && i < size; i++) {
    if (a[i] > max) {
      max = a[i];
    }
  }

  *res = max;

  return 0;
}

int ind_stochastic(double const *high, double const *low, double const *close, size_t size, int period, double *outarr)
{
  double minlow;
  double maxhigh;
  double diff1;
  double diff2;
  double fastk;
  int pos;

  for (pos = period; pos < size; pos++) {
    stats_range_min(low, size, pos - period, pos /* stop index */, &minlow);
    printf("%.2f\n", minlow);

    stats_range_max(high, size, pos - period, pos /* stop index */, &maxhigh);
    printf("%.2f\n", maxhigh);

    printf("%.2f\n", close[pos - 1]);
    
    diff1 = close[pos - 1] - minlow;
    diff2 = maxhigh - minlow;

    fastk = (diff1 / diff2) * 100;
    printf("%.2f\n", fastk);

    outarr[pos] = fastk;
  }

  return 0;
}
