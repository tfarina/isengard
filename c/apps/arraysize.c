#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(void) {
  int numbers[] = {0, 1, 2, 3};

  printf("%ld\n", ARRAY_SIZE(numbers));

  return 0;
}
