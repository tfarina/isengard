#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrdup.h"

int main(int argc, char **argv) {
  const char *src = "hello, world";
  char *dst;

  dst = f_strdup(src);

  printf("%s\n", dst);

  free(dst);

  return 0;
}
