#ifndef PIVOT_H_
#define PIVOT_H_

/**
 * The pivot point is the point in which the market sentiment changes from
 * bearish to bullish.
 */

float pivot_point(float high, float low, float close);

float resistance1(float pivot_point, float low);

float resistance2(float pivot_point, float r1, float s1);

float resistance3(float pivot_point, float high, float low);

float support1(float pivot_point, float high);

float support2(float pivot_point, float r1, float s1);

float support3(float pivot_point, float high, float low);

#endif /* PIVOT_H_ */
