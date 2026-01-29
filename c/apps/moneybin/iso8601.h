#ifndef ISO8601_H
#define ISO8601_H

/**
 * Converts a date/time in ISO 8601 format to a "struct tm".
 */
time_t str_to_iso8601(char const *str);

/**
 * Converts a struct tm into an ISO 8601 standard date.
 */
void iso8601_to_str(time_t t, char *dst, int len);

#endif /* ISO8601_H */
