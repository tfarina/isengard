#ifndef STATS_H_
#define STATS_H_

#include <stddef.h>

/**
 * Calculates the minimum value in the specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_min(double const *data, size_t size);

/**
 * Calculates the maximum value in the specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_max(double const *data, size_t size);

/**
 * Calculates the sum of all values in the specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_sum(double const *data, size_t size);

/**
 * Calculates the arithmetic mean of the values in the specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_avg(double const *data, size_t size);

/**
 * Calculates the variance based on a sample of the values in the
 * specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_var(double const *data, size_t size);

/**
 * Calculates the standard deviation based on a sample of the values in the
 * in the specified array.
 *
 * @param data The array.
 * @param size The array size.
 *
 */
double stats_stdev(double const *data, size_t size);

/**
 * Calculates the covariance value, given the two arrays of doubles.
 *
 * @param data1 The first array.
 * @param size1 The first array size.
 * @param data2 The second array.
 * @param size2 The second array size.
 *
 */
double stats_covar(double const *data1, size_t size1,
                   double const *data2, size_t size2);

/**
 * Calculates the Pearson correlation coefficient, given the two arrays of
 * doubles.
 *
 * @param data1 The first array.
 * @param size1 The first array size.
 * @param data2 The second array.
 * @param size2 The second array size.
 *
 */
double stats_corr(double const *data1, size_t size1,
                  double const *data2, size_t size2);

/**
 * Calculates the minimum value in the specified array for the given interval.
 *
 * @param data The array.
 * @param size The array size.
 * @param start The index where it should start.
 * @param stop The index where it should stop
 * @param res The output result.
 */
int stats_range_min(double const *data, size_t size, int start, int stop, double *res);

/**
 * Calculates the maximum value in the specified array for the given interval.
 *
 * @param data The array.
 * @param size The array size.
 * @param start The index where it should start.
 * @param stop The index where it should stop
 * @param res The output result.
 */
int stats_range_max(double const *data, size_t size, int start, int stop, double *res);

#endif /* STATS_H_ */
