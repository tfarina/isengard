#ifndef STATS_H_
#define STATS_H_

/**
 * Calculates the minimum value in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_min(double const *a, int size, double *res);

/**
 * Calculates the maximum value in the specified array, or NAN if the
 * array is empty.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_max(double const *a, int size, double *res);

/**
 * Calculates the sum of all values in the specified array.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_sum(double const *a, int size, double *res);

/**
 * Calculates the arithmetic mean of the values in the specified array.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_avg(double const *a, int size, double *res);

/**
 * Calculates the variance based on a sample of the values in the
 * specified array.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_var(double const *a, int size, double *res);

/**
 * Calculates the standard deviation based on a sample of the values in the
 * in the specified array.
 *
 * @param a The array.
 * @param size The array size.
 * @param res  The output where the result is stored.
 */
int stats_stdev(double const *a, int size, double *res);

#endif /* STATS_H_ */
