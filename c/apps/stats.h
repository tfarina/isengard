#ifndef STATS_H_
#define STATS_H_

#include <stddef.h>

/**
 * Returns the minimum of the entries in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 */
double stats_min(double *a, size_t size);

/**
 * Returns the maximum of the entries in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 */
double stats_max(double *a, size_t size);

/**
 * Returns the sum of the values in the specified array.
 *
 * @param a The array.
 */
double stats_sum(double *a, size_t size);

/**
 * Returns the arithmetic mean of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_average(double *a, size_t size);

/**
 * Returns the sample variance of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_var(double *a, size_t size);

/**
 * Returns the sample standard deviation of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_stdev(double *a, size_t size);

#endif /* STATS_H_ */
