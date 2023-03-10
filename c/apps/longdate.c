#include <time.h>
#include <stdio.h>

#define DATEFORMAT "%A, %B %d, %Y"

int
main(void)
{
  time_t now;
  struct tm *curtime;
  char datebuf[32];

  now = time(NULL);
  curtime = localtime(&now);

  strftime(datebuf, sizeof(datebuf), DATEFORMAT, curtime);

  printf("%s\n", datebuf);

  return 0;
}
