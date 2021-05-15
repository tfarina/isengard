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

double pivot_classic_pp(double high, double low, double close)
{
  return (high + low + close) / 3;
}

double pivot_classic_r1(double pivot_point, double low)
{
  return (2 * pivot_point) - low;
}

double pivot_classic_r2(double pivot_point, double high, double low)
{
  return pivot_point + (high - low);
}

double pivot_classic_r3(double pivot_point, double high, double low)
{
  return high + 2 * (pivot_point - low);
}

double pivot_classic_s1(double pivot_point, double high)
{
  return (2 * pivot_point) - high;
}

double pivot_classic_s2(double pivot_point, double high, double low)
{
  return pivot_point - (high - low);
}

double pivot_classic_s3(double pivot_point, double high, double low)
{
  return low - 2 * (high - pivot_point);
}
