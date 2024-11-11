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
  char *dst;

  if (!str) {
    return NULL;
  }

  dst = malloc((strlen(str) + 1) * sizeof(char));
  if (!dst) {
    return NULL;
  }

  strcpy(dst, str);

  return dst;
}
