#include "fstrdup.h"

#include <stdlib.h>
#include <string.h>

/**
 * Duplicate a string.
 *
 * @param[in] src String to be duplicated
 * @return A pointer to the duplicated string or `NULL` if no memory was
 * available.
 *
 * @remark
 *    The returned string should be freed when no longer needed.
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

  strcpy(dst, src);

  return dst;
}
