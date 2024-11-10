#include "xstring.h"

#include <stdlib.h>
#include <string.h>

/**
 * Duplicates a string.
 *
 * @param[in] str String to be duplicated
 * @return A pointer to the duplicated string or `NULL` if no memory was
 * available.
 *
 * @remark
 *    The returned string should be freed when no longer needed.
 */
char *
xstrdup(char const *str)
{
  size_t len;
  char *dst;

  if (!str) {
    return NULL;
  }

  len = strlen(str) + 1;
  dst = malloc(sizeof(char) * len);
  if (!dst) {
    return NULL;
  }

  strcpy(dst, str);

  return dst;
}
