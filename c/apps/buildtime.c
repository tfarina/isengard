#include <stdio.h>

#define BUILD_DATE __DATE__ " " __TIME__

static char const timestamp[] = BUILD_DATE;

int main(void)
{
  printf("Build date: %s\n", timestamp);

  return 0;
}
