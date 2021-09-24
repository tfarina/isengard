#include <stdio.h>

#include "arraysize.h"

static char const *period[] = { "1d", "5d", "1mo", "3mo", "6mo", "1y", "2y", "5y", "10y", "ytd", "max" };

int main(void)
{
  size_t period_size;
  int i;

  period_size = ARRAY_SIZE(period);

  for (i = 0; i < period_size; i++) {
    printf("%s ", period[i]);
  }

  putc('\n', stdout);

  return 0;
}
