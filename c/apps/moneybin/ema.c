#include "ema.h"

/**
 * Calculates a exponential moving average.
 *
 */
void ema(double const *arr, size_t size, int period, double *outarr)
{
  float val;
  int i;
  float k = 2 / ((float)period + 1); /* 2 is the smoothing factor */

  val = *arr;
  *outarr++ = val;

  for (i = 1; i < size; i++) {
    /* EMA = (close - prev_day_ema) * k + prev_day_ema */
    val = (*(arr + i) - val) * k + val;

    *outarr++ = val;
  }
}
