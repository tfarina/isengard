#include <stdio.h>
#include <stdlib.h>

double f(double x) {
  double y;

  y = x * x;

  return y;
}

int main(int argc, char **argv) {
  double x = 2;
  double y = 0;

  y = f(x);

  printf("%.1f\n", y);

  return 0;
}
