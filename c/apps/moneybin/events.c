#include <stdio.h>

#include "arraysize.h"

static char const *events[] = { "history", "div", "split", "capital" };

int main(void)
{
  size_t events_size;
  int i;

  events_size = ARRAY_SIZE(events);

  for (i = 0; i < events_size; i++) {
    printf("%s\n", events[i]);
  }

  return 0;
}
