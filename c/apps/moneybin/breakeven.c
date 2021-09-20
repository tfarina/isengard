#include <stdio.h>

#include "finance.h"

int
main(int argc, char **argv)
{
  double be;
  double buy_price;
  double commission;
  int num_shares;

  buy_price = 10.00;
  commission = 4.50;
  num_shares = 100;  

  be = break_even(buy_price, commission, num_shares);

  printf("Break Even Price: %.2f\n", be);

  return 0;
}
