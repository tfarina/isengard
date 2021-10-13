#include "strutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Duplicate a string.
 *
 * The returned string should be freed when no longer needed.
 *
 * @param[in] src String to be duplicated
 * @return Pointer to a newly-allocated string containing a copy of `src`
 *         or `NULL` if the string cannot be allocated.
 */
char *f_strdup(char const *src) {
  size_t len;
  char *dst;

  if (src == 0) {
    return (char *) 0;
  }

  len = strlen(src) + 1;
  dst = malloc(sizeof(char) * len);
  if (dst == 0) {
    return (char *) 0;
  }

  return strcpy(dst, src);
}
