#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

typedef struct
{
  long	date;
  long	time;
} timestamp_t;

int set_date(int unsigned year,
             int unsigned month,
	     int unsigned day,
	     timestamp_t *timestamp);

int next_day(timestamp_t *timestamp);

int unsigned get_year(timestamp_t const *timestamp);
int unsigned get_month(timestamp_t const *timestamp);
int unsigned get_day(timestamp_t const *timestamp);

void ta_getdate(timestamp_t const *timestamp,
		int unsigned *year,
		int unsigned *month,
		int unsigned *day);

#endif  /* TIMESTAMP_H_ */
