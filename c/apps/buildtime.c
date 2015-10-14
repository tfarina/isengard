#include <stdio.h>

int main(void) {
  char b[] = "Built on: " __DATE__ " " __TIME__;

  printf("%s\n", b);

  return 0;
}
