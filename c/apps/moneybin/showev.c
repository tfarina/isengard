#include <stdio.h>

#include "arraysize.h"

static char const *show_events[] = { "Historical Prices", "Dividends Only", "Stock Splits", "Capital Gain" };

int main(void)
{
  size_t show_events_size;
  int i;

  show_events_size = ARRAY_SIZE(show_events);

  for (i = 0; i < show_events_size; i++) {
    printf("%s\n", show_events[i]);
  }

  return 0;
}
