#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xstring.h"

int main(int argc, char **argv) {
  const char *src = "hello, world";
  char *dst;

  dst = xstrdup(src);

  printf("%s\n", dst);

  free(dst);

  return 0;
}
