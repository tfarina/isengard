#ifndef STATS_H_
#define STATS_H_

#include <stddef.h>

/**
 * Returns the sum of the values in the input array.
 */
double stats_sum(double a[], size_t size);

/**
 * Returns the arithmetic mean of the entries in the input array.
 */
double stats_mean(double a[], size_t size);

/**
 * Returns the population variance of the entries in the input array.
 */
double stats_variance(double a[], size_t size);

#endif /* STATS_H_ */
