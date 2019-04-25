#include <stdio.h>

static char const BUILT_STR[] = __DATE__ " " __TIME__;

int main(void) {
  printf("Built on: %s\n", BUILT_STR);

  return 0;
}
