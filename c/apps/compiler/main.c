#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffileutils.h"
#include "lexer.h"

int main(int argc, char **argv) {
  size_t len;
  char *buf;

  if (argc < 2) {
    fprintf(stderr, "usage: lexer <filename>\n");
    return 1;
  }

  buf = f_read_file(argv[1], &len);

  lex(buf);

  free(buf);

  return 0;
}
