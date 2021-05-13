#ifndef PIVOT_H_
#define PIVOT_H_

/**
 * The pivot point is the point in which the market sentiment changes from
 * bearish to bullish.
 */

double classic_pp_pivot_point(double high, double low, double close);

double classic_pp_resistance1(double pivot_point, double low);

double classic_pp_resistance2(double pivot_point, double high, double low);

double classic_pp_resistance3(double pivot_point, double high, double low);

double classic_pp_support1(double pivot_point, double high);

double classic_pp_support2(double pivot_point, double high, double low);

double classic_pp_support3(double pivot_point, double high, double low);

#endif /* PIVOT_H_ */
