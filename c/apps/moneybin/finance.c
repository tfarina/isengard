#include "finance.h"

double break_even(double buy_price, double commission, int num_shares)
{
  return ((buy_price * num_shares) + (2 * commission)) / num_shares;
}
