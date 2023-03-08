#include <time.h>
#include <stdio.h>

int
main(void)
{
  time_t now;
  struct tm *curtime;

  char *months[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  };

  now = time(NULL);
  curtime = localtime(&now);

  printf("%s\n", months[curtime->tm_mon]);

  return 0;
}
