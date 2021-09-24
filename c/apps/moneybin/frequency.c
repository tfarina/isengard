#include <stdio.h>

#include "arraysize.h"

static char const *frequency[] = { "Daily", "Weekly", "Monthly" };

int main(void)
{
  size_t frequency_size;
  int i;

  frequency_size = ARRAY_SIZE(frequency);

  for (i = 0; i < frequency_size; i++) {
    printf("%s\n", frequency[i]);
  }

  return 0;
}
