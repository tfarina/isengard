#include <stdio.h>

#include "finance.h"

int main(int argc, char **argv) {
  double change = 0.0;

  change = percentage_change(2.5, 2.61);

  printf("%.2f%%\n", change);

  return 0;
}
