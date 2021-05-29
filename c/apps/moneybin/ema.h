#ifndef EMA_H_
#define EMA_H_

#include <stddef.h>

/**
 * Exponential Moving Average
 *
 * @param[in]  arr
 * @param[in]  size
 * @param[in]  lookback_period
 * @param[out] outarr
 *
 * @return exponential moving average
 */
void ema(double const *arr, size_t size, int lookback_period, double *outarr);

#endif /* EMA_H_ */
