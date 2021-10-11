#include "strutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Duplicates a string.
 *
 * The returned string should be freed when no longer needed.
 *
 * @param[in] src (nullable): the string to duplicate
 * @return    allocated memory
 */
char *f_strdup(char const *src) {
  size_t len;
  char *dst;

  if (src == 0) {
    return (char *) 0;
  }

  len = strlen(src) + 1;
  dst = malloc(sizeof(char) * len);

  return strcpy(dst, src);
}
