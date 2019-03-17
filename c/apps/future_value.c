#include <stdio.h>

#include "finance.h"

int main(int argc, char **argv)
{
  double fv;

  fv = future_value(2.20, 0.0972, 10);

  printf("Future Value: $%.2f\n", fv);

  return 0;
}
