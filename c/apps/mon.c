#include <time.h>
#include <stdio.h>

int
main(void)
{
  time_t now;
  struct tm *curtime;

  now = time(NULL);
  curtime = localtime(&now);

  printf("%d\n", curtime->tm_mon + 1);

  return 0;
}
