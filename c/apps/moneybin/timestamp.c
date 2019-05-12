#include "timestamp.h"

#include "sfldate.h"

static void move_delta_day(long *date, int delta)
{
  long days;

  days = date_to_days(*date);
  days += delta;
  *date = days_to_date(days);
}

int set_date(int unsigned year,
             int unsigned month,
	     int unsigned day,
	     timestamp_t *timestamp) {
  long cc;

  if (timestamp == NULL) {
    return -1;
  }

  if (year <= 99) {
    if (year >= 11) {
      cc = 19;
    } else {
      cc = 20;
    }
  } else {
    cc = year / 100;
  }

  timestamp->date = MAKE_DATE(cc, year % 100, month, day);

  return 0;
}

int next_day(timestamp_t *timestamp) {
  if (timestamp == NULL) {
    return -1 /*TA_BAD_PARAM */;
  }

  move_delta_day(&timestamp->date, 1);

  return 0;
}

int unsigned get_year(timestamp_t const *timestamp) {
  if (timestamp == NULL) {
    return 0;
  }

  return GET_CCYEAR(timestamp->date);
}

int unsigned get_month(timestamp_t const *timestamp) {
  if (timestamp == NULL) {
    return 0;
  }

  return GET_MONTH(timestamp->date);
}

int unsigned get_day(timestamp_t const *timestamp) {
  if (timestamp == NULL) {
    return 0;
  }

  return GET_DAY(timestamp->date);
}
