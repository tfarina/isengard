#include "strutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Duplicates a string.
 *
 * @param src (nullable): the string to duplicate
 */
char *f_strdup(char const *src) {
  char *dst;
  size_t len;

  if (!src) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = malloc(len);

  return memcpy(dst, src, len);
}
