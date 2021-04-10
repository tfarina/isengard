#ifndef SMA_H_
#define SMA_H_

#include <stddef.h>

/**
 * Simple Moving Average
 *
 * @param[in] arr
 * @param[in] size
 * @param[in] window
 * @param[out] outarr
 *
 * @return simple moving average
 */
void smovavg(double const *arr, size_t size, int window, double *outarr);

#endif /* SMA_H_ */
