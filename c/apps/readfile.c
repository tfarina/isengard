#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffileutils.h"

int main(int argc, char **argv) {
  size_t len;
  char *buf;

  if (argc != 2) {
    fprintf(stderr, "usage: readfile <filename>\n");
    exit(EXIT_FAILURE);
  }

  buf = f_read_file(argv[1], &len);
  if (!buf) {
    free(buf);
    return -1;
  }

  fwrite(buf, 1, len, stdout);

  free(buf);

  return 0;
}
