#ifndef STATS_H_
#define STATS_H_

#include <stddef.h>

/**
 * Returns the sum of the values in the specified array.
 */
double stats_sum(double a[], size_t size);

/**
 * Returns the arithmetic mean of the entries in the specified array.
 */
double stats_mean(double a[], size_t size);

/**
 * Returns the sample variance of the entries in the specified array.
 */
double stats_variance(double a[], size_t size);

/**
 * Returns the sample standard deviation of the entries in the specified array.
 */
double stats_stdev(double a[], size_t size);

#endif /* STATS_H_ */
