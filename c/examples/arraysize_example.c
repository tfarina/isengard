#include <stdio.h>

#include "base.h"

int main(void) {
  int numbers[] = {0, 1, 2, 3};

  printf("%u\n", ARRAY_SIZE(numbers));

  return 0;
}
