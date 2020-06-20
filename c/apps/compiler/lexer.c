#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffileutils.h"

static void lex(const char *input)
{
  const char *c;

  for (c = input; *c; ++c) {
    switch (*c) {
      case '[':
        printf("<'[', LEFT_BRACKET>\n");
        break;
      case ']':
        printf("<']', RIGHT_BRACKET>\n");
        break;
      case '{':
        printf("<'{', LEFT_BRACE>\n");
        break;

      case '}':
        printf("<'}', RIGHT_BRACE>\n");
        break;
    }
  }
}

int main(int argc, char **argv) {
  size_t len;
  char *buf;

  if (argc != 2) {
    fprintf(stderr, "usage: lexer <filename>\n");
    exit(EXIT_FAILURE);
  }

  buf = f_read_file(argv[1], &len);

  lex(buf);

  free(buf);

  return 0;
}
