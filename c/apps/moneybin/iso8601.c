#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>
#include <string.h>

/**
 * ISO 8601 Date and Time Format:
 *
 * YYYY-MM-DD
 *
 * https://www.iso.org/iso-8601-date-and-time-format.html
 */
time_t str_to_iso8601(char const *str)
{
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

void iso8601_to_str(time_t t, char *dst, int len)
{
  struct tm *time_tm;

  if (dst == NULL) {
    return;
  }

  time_tm = localtime(&t);
  strftime(dst, len, "%Y-%m-%d", time_tm);
}
