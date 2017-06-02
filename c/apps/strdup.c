#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Duplicate a string.
 *
 */
static char *xstrdup(const char *src) {
  char *dst;
  size_t len;

  if (!src) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = malloc(len);

  return memcpy(dst, src, len);
}

int main(int argc, char **argv) {
  const char *src = "hello, world";
  char *dst;

  dst = xstrdup(src);

  printf("%s\n", dst);

  free(dst);

  return 0;
}
