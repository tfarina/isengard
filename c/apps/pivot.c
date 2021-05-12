/**
 * The pivot point is calculated from the high (H), low (L) and close (C)
 * of the previous day.
 *
 * Pivot point = P = (H + L + C) / 3
 *
 * Resistance:
 *
 * First area of resistance = R1 = 2P - L
 * Second area of resistance = R2 = P + (R1 - S1)
 * Third area of resistance = R3 = H + 2 * (P - L)
 *
 * Support:
 *
 * First area of support = S1 = 2P - H
 * Second area of support = S2 = P - (R1 - S1)
 * Third area of support = S3 = L - 2 * (H - P)
 */

#include "pivot.h"

float classic_pp_pivot_point(float high, float low, float close)
{
  return (high + low + close) / 3;
}

float classic_pp_resistance1(float pivot_point, float low)
{
  return (2 * pivot_point) - low;
}

float classic_pp_resistance2(float pivot_point, float high, float low)
{
  return pivot_point + (high - low);
}

float classic_pp_resistance3(float pivot_point, float high, float low)
{
  return high + 2 * (pivot_point - low);
}

float classic_pp_support1(float pivot_point, float high)
{
  return (2 * pivot_point) - high;
}

float classic_pp_support2(float pivot_point, float high, float low)
{
  return pivot_point - (high - low);
}

float classic_pp_support3(float pivot_point, float high, float low)
{
  return low - 2 * (high - pivot_point);
}
