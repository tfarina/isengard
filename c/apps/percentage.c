#include <stdio.h>

static double percentage_change(double old_value, double new_value) {
  double increase = new_value - old_value;

  return (increase / old_value) * 100;
}

int main(int argc, char **argv) {
  double change = percentage_change(2.5, 2.61);

  printf("%f%%\n", change);

  return 0;
}
