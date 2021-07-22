#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>
#include <string.h>
#include <stdio.h>

/**
 * ISO 8601 Date and Time Format:
 *
 * YYYY-MM-DD
 *
 * https://www.iso.org/iso-8601-date-and-time-format.html
 */
static time_t str_to_time(char const *str) {
  struct tm time_tm;
  time_t unixtime;

  if ((str == NULL) || (*str == '\0')) {
    return (time_t) -1;
  }

  memset(&time_tm, 0, sizeof(struct tm));

  strptime(str, "%Y-%m-%d", &time_tm);

  unixtime = mktime(&time_tm);

  return unixtime;
}

static void time_to_str(time_t t, char *dst, int len) {
  struct tm *time_tm;

  if (dst == NULL) {
    return;
  }

  time_tm = localtime(&t);
  strftime(dst, len, "%D", time_tm);
}

int main(int argc, char **argv) {
  char *start;
  time_t start_time;
  char samestart[32];
  
  if (argc < 2) {
    fputs("usage: strtotime YYYY-MM-DD\n", stderr);
    return 1;
  }

  start = argv[1];

  start_time = str_to_time(start);

  time_to_str(start_time, samestart, sizeof(samestart));

  printf("%s\n", samestart);

  return 0;
}
