#ifndef STATS_H_
#define STATS_H_

#include <stddef.h>

/**
 * Returns the minimum of the entries in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 */
int stats_min(double const *a, size_t size, double *res);

/**
 * Returns the maximum of the entries in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 */
int stats_max(double const *a, size_t size, double *res);

/**
 * Returns the sum of the values in the specified array.
 *
 * @param a The array.
 */
int stats_sum(double const *a, size_t size, double *res);

/**
 * Returns the arithmetic mean of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_average(double const *a, size_t size);

/**
 * Returns the sample variance of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_var_sam(double const *a, size_t size);

/**
 * Returns the sample standard deviation of the entries in the specified array.
 *
 * @param a The array.
 */
double stats_stdev_sam(double const *a, size_t size);

#endif /* STATS_H_ */
