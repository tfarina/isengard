#include <stdio.h>

#include "financial.h"

int main(void)
{
  double principal, rate, periods;
  double pv;

  principal = 100;
  rate = 0.05; /* 5% */
  periods = 5;

  pv = present_value(principal, rate, periods);

  printf("Present Value: $%.2f\n", pv);

  return 0;
}
