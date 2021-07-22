#ifndef _ISO8601_H_INCLUDED_
#define _ISO8601_H_INCLUDED_

time_t str_to_time(char const *str);

void time_to_str(time_t t, char *dst, int len);

#endif  /* !defined(_ISO8601_H_INCLUDED_) */
