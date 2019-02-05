#include <stdio.h>

static double percentage_change(double old_value, double new_value) {
  double diff = 0.0;

  diff = new_value - old_value;

  return (diff / old_value) * 100;
}

int main(int argc, char **argv) {
  double change = 0.0;

  change = percentage_change(2.5, 2.61);

  printf("%.2f%%\n", change);

  return 0;
}
