#ifndef SMA_H_
#define SMA_H_

#include <stddef.h>

/**
 * Simple Moving Average
 *
 * @param[in] input
 * @param[in] size
 * @param[in] period
 * @param[out] output
 *
 * @return simple moving average
 */
int ind_sma(double const *input, size_t size, int period, double *output);

#endif /* SMA_H_ */
