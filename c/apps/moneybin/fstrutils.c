#include "fstrutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Duplicates a string.
 *
 * The returned string should be freed when no longer needed.
 *
 * @param src (nullable): the string to duplicate
 */
char *f_strdup(char const *src) {
  size_t len;
  char *dst;

  if (src == 0) {
    return (char *) 0;
  }

  len = strlen(src) + 1;
  dst = malloc(len);

  return strcpy(dst, src);
}
