#include <stdio.h>

#include "arraysize.h"

static char const *interval[] = { "1d", "1wk", "1mo" };

int main(void)
{
  size_t interval_size;
  int i;

  interval_size = ARRAY_SIZE(interval);

  for (i = 0; i < interval_size; i++) {
    printf("%s\n", interval[i]);
  }

  return 0;
}
