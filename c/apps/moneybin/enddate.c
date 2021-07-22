#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
  time_t now;
  struct tm* now_tm;
  char end_date_str[11];

  now = time(NULL); /* get time right now */
  now_tm = localtime(&now);
  strftime(end_date_str, sizeof(end_date_str), "%Y-%m-%d", now_tm);

  printf("%s\n", end_date_str);

  return 0;
}
