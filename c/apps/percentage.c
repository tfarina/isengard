#include <stdio.h>

static int percentage_change(int old_value, int new_value) {
  int increase = new_value - old_value;

  return (increase / old_value) * 100;
}

int main(int argc, char **argv) {
  int change = percentage_change(2, 10);

  printf("%d%%\n", change);

  return 0;
}
