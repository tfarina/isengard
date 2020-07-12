#include <stdio.h>

#include "financial.h"

int main(void)
{
  double principal, rate, periods;
  double pv;

  principal = 2.20;
  rate = 0.0972;
  periods = 10;

  pv = present_value(principal, rate, periods);

  printf("Present Value: $%.2f\n", pv);

  return 0;
}
