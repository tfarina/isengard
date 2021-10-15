#include "finance.h"

double break_even(double buy_price, double commission, int num_shares)
{
  return ((buy_price * num_shares) + (2 * commission)) / num_shares;
}

void daily_returns(double const *data, size_t size, double *res)
{
  size_t i;

  /* The returns start at the second day. */
  for (i = 1; i < size; i++) {
    res[i - 1] = (data[i] - data[i - 1]) / data[i - 1];
  }
}
