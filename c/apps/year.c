#include <time.h>
#include <stdio.h>

int
main(void)
{
  time_t now;
  struct tm *curtime;

  now = time(NULL);
  curtime = localtime(&now);

  printf("%d\n", curtime->tm_year + 1900);

  return 0;
}
