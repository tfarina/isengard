#include <time.h>
#include <stdio.h>

#define TIMEFORMAT "%I:%M %p"

int
main(void)
{
  time_t now;
  struct tm *curtime;
  char timebuf[32];

  now = time(NULL);
  curtime = localtime(&now);

  strftime(timebuf, sizeof(timebuf), TIMEFORMAT, curtime);

  printf("%s\n", timebuf);

  return 0;
}
