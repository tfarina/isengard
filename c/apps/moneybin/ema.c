#include "ema.h"

/**
 * Calculates a exponential moving average.
 *
 */
void ema(double const *arr, size_t size, int lookback_period, double *outarr)
{
  float val;
  int i;
  float k = 2 / ((float)lookback_period + 1); /* 2 is the smoothing factor */

  val = *arr;
  *outarr++ = val;

  for (i = 1; i < size; i++) {
    /* EMA = [close - EMA(previous day)] * multiplier + EMA(previous day)*/
    val = (*(arr + i) - val) * k + val;

    *outarr++ = val;
  }
}
