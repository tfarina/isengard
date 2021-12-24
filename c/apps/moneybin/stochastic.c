#include "ta.h"

#include <stdio.h>

/**
 * Calculates the minimum value in the specified array for the given interval.
 *
 * @param data The array.
 * @param size The array size.
 * @param start The index where it should start.
 * @param stop The index where it should stop
 * @param res The output result.
 */
static int stats_range_min(double const *data, size_t size, int start, int stop, double *res)
{
  double min;
  int i;

  if (size <= 0) {
    return -1;
  }

  min = data[start];

  for (i = start; i <= stop && i < size; i++) {
    if (data[i] < min) {
      min = data[i];
    }
  }

  *res = min;

  return 0;
}

/**
 * Calculates the maximum value in the specified array for the given interval.
 *
 * @param data The array.
 * @param size The array size.
 * @param start The index where it should start.
 * @param stop The index where it should stop
 * @param res The output result.
 */
static int stats_range_max(double const *data, size_t size, int start, int stop, double *res)
{
  double max;
  int i;

  if (size <= 0) {
    return -1;
  }

  max = data[start];

  for (i = start; i <= stop && i < size; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }

  *res = max;

  return 0;
}

/**
 * Stochastic Oscillator
 *
 * The stochastic oscillator is a momentum indicator. It was developed by
 * George Lane in the late 1950s.
 *
 * Equation:
 *
 *      (C - L)
 * %K = ------- x 100
 *      (H - L)
 *
 * Where:
 *
 *  - C is the current closing price
 *  - H is the highest high over the lookback period
 *  - L is the lowest low over the lookback period
 *
 * @param[in]  arr
 * @param[in]  size
 * @param[in]  period
 * @param[out] output
 * @return 0 on success, negative number (< 0) on error.
 */
int ind_stochastic(double const *high, double const *low, double const *close, size_t size, int period, double *output)
{
  double minlow;
  double maxhigh;
  double diff1;
  double diff2;
  double fastk;
  int pos;

  for (pos = period; pos < size; pos++) {
    stats_range_min(low, size, pos - period, pos /* stop index */, &minlow);
#if 0
    printf("%.2f\n", minlow);
#endif

    stats_range_max(high, size, pos - period, pos /* stop index */, &maxhigh);
#if 0
    printf("%.2f\n", maxhigh);

    printf("%.2f\n", close[pos - 1]);
#endif
    
    diff1 = close[pos - 1] - minlow;
    diff2 = maxhigh - minlow;

    fastk = (diff1 / diff2) * 100;
#if 0
    printf("%.2f\n", fastk);
#endif

    output[pos] = fastk;
  }

  return 0;
}
