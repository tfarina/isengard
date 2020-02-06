#include <stdio.h>

#define BUILD_DATE __DATE__ " " __TIME__

static char const timestamp[] = BUILD_DATE;

int main(void)
{
  printf("Build Time: %s\n", timestamp);

  return 0;
}
