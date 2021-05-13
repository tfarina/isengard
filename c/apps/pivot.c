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

double classic_pp_pivot_point(double high, double low, double close)
{
  return (high + low + close) / 3;
}

double classic_pp_resistance1(double pivot_point, double low)
{
  return (2 * pivot_point) - low;
}

double classic_pp_resistance2(double pivot_point, double high, double low)
{
  return pivot_point + (high - low);
}

double classic_pp_resistance3(double pivot_point, double high, double low)
{
  return high + 2 * (pivot_point - low);
}

double classic_pp_support1(double pivot_point, double high)
{
  return (2 * pivot_point) - high;
}

double classic_pp_support2(double pivot_point, double high, double low)
{
  return pivot_point - (high - low);
}

double classic_pp_support3(double pivot_point, double high, double low)
{
  return low - 2 * (high - pivot_point);
}
