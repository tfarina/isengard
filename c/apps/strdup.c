#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *src) {
  char *dst;
  size_t len;

  if (!src) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = malloc(len);

  memcpy(dst, src, len);

  return dst;
}

int main(int agrc, char **argv) {
  const char *src = "hello, world";
  char *dst;

  dst = strdup(src);

  printf("%s\n", dst);

  free(dst);

  return 0;
}
