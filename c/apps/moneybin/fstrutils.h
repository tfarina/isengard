#ifndef STRUTILS_H_
#define STRUTILS_H_

#define str_equal(a, b) (strcmp((a), (b)) == 0)

int str_is_null_or_empty(char const *str);
int str_startswith(char const *str, char const *prefix);
char *str_substring(char const *str, int start, int end);
char *str_strip_leading(char *str);
char *str_strip_trailing(char *str);
char *str_strip(char *str);

#endif  /* STRUTILS_H_ */
