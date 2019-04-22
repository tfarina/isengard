#include <stdio.h>

#include "financial.h"

int main(int argc, char **argv)
{
  double gr;

  gr = growth_rate(310, 205, 10);

  printf("Compound Annual Growth Rate: %.2f%%\n", gr * 100);

  return 0;
}
