#ifndef PIVOT_H_
#define PIVOT_H_

/**
 * The pivot point is the point in which the market sentiment changes from
 * bearish to bullish.
 */

float classic_pp_pivot_point(float high, float low, float close);

float classic_pp_resistance1(float pivot_point, float low);

float classic_pp_resistance2(float pivot_point, float r1, float s1);

float classic_pp_resistance3(float pivot_point, float high, float low);

float classic_pp_support1(float pivot_point, float high);

float classic_pp_support2(float pivot_point, float high, float low);

float classic_pp_support3(float pivot_point, float high, float low);

#endif /* PIVOT_H_ */
