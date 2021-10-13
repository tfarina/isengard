#include <stdio.h>

#include "percentage.h"

int
main(int argc, char **argv)
{
  double variation;

  variation = percentage_change(2.5, 2.61);

  printf("%.2f%%\n", variation);

  return 0;
}
