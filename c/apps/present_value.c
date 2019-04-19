#include <stdio.h>

#include "financial.h"

int main(int argc, char **argv)
{
  double pv;

  pv = present_value(2.20, 0.0972, 10);

  printf("Present Value: $%.2f\n", pv);

  return 0;
}
