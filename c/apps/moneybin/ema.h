#ifndef EMA_H_
#define EMA_H_

#include <stddef.h>

/**
 * Exponential Moving Average
 *
 * @param[in]  input
 * @param[in]  size
 * @param[in]  period
 * @param[out] output
 *
 * @return exponential moving average
 */
int ind_ema(double const *input, size_t size, int period, double *output);

#endif /* EMA_H_ */
