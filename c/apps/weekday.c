#include <time.h>
#include <stdio.h>

int
main(void)
{
  time_t now;
  struct tm *curtime;

  char *week_day_names[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };

  now = time(NULL);
  curtime = localtime(&now);

  printf("%s\n", week_day_names[curtime->tm_wday]);

  return 0;
}
