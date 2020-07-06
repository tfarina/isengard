#ifndef SMA_H_
#define SMA_H_

#include <stddef.h>

void smovavg(double const *arr, size_t size, int window, double *outarr);

#endif /* SMA_H_ */
