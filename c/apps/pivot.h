#ifndef PIVOT_H_
#define PIVOT_H_

/**
 * The pivot point is the point in which the market sentiment changes from
 * bearish to bullish.
 */

double pivot_classic_pp(double high, double low, double close);

double pivot_classic_r1(double pivot_point, double low);

double pivot_classic_r2(double pivot_point, double high, double low);

double pivot_classic_r3(double pivot_point, double high, double low);

double pivot_classic_s1(double pivot_point, double high);

double pivot_classic_s2(double pivot_point, double high, double low);

double pivot_classic_s3(double pivot_point, double high, double low);


double pivot_fibonacci_pp(double high, double low, double close);

double pivot_fibonacci_r1(double pivot_point, double high, double low);

double pivot_fibonacci_r2(double pivot_point, double high, double low);

double pivot_fibonacci_r3(double pivot_point, double high, double low);

double pivot_fibonacci_s1(double pivot_point, double high, double low);

double pivot_fibonacci_s2(double pivot_point, double high, double low);

double pivot_fibonacci_s3(double pivot_point, double high, double low);


#endif /* PIVOT_H_ */
