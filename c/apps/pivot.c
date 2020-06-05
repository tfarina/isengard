#include "pivot.h"

/**
 * Pivot point = P = (H + L + C) / 3
 * First area of resistance = R1 = 2P - L
 * Second area of resistance = R2 = P + (R1 - S1)
 * Third area of resistance = R3 = H + 2 * (P - L)
 * First area of support = S1 = 2P - H
 * Second area of support = S2 = P - (R1 - S1)
 * Third area of support = S3 = L - 2 * (H - P)
 */

float pivot_point(float high, float low, float close)
{
  return (high + low + close) / 3;
}

float resistance1(float pivot_point, float low)
{
  return (2 * pivot_point) - low;
}

float resistance2(float pivot_point, float r1, float s1)
{
  return pivot_point + (r1 - s1);
}

float resistance3(float pivot_point, float high, float low)
{
  return high + 2 * (pivot_point - low);
}

float support1(float pivot_point, float high)
{
  return (2 * pivot_point) - high;
}

float support2(float pivot_point, float r1, float s1)
{
  return pivot_point - (r1 - s1);
}

float support3(float pivot_point, float high, float low)
{
  return low - 2 * (high - pivot_point);
}
