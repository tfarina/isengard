#ifndef EMA_H_
#define EMA_H_

#include <stddef.h>

/**
 * Exponential Moving Average
 *
 * @param[in]  arr
 * @param[in]  size
 * @param[in]  period
 * @param[out] outarr
 *
 * @return exponential moving average
 */
void ema(double const *arr, size_t size, int period, double *outarr);

#endif /* EMA_H_ */
