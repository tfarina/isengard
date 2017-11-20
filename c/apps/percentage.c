#include <stdio.h>

int percentage_increase(int old_value, int new_value) {
  int increase = new_value - old_value;

  return (increase / old_value) * 100;
}

int main(int argc, char **argv) {
  int increase = percentage_increase(2, 10);

  printf("%d%%\n", increase);

  return 0;
}
