#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrutils.h"
#include "test.h"

static int
dump_b3_symbols(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  char symbol[12];

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  memset(symbol, '\0', sizeof(symbol));

  /* Loops through the file reading line by line. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    strncpy(symbol, linebuf + 2, 12);
    symbol[12] = 0;
    printf("%s\n", symbol);
  }

  return 0;
}

int
main(int argc, char **argv)
{
  int retval;
  char *filename;

  if (argc != 2) {
    fprintf(stderr, "usage: b3symbols TITULOS_NEGOCIAVEIS.txt\n");
    return 1;
  }

  filename = f_strdup(argv[1]);

  if (!test_exists(filename)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = dump_b3_symbols(filename);
  if (retval < 0) {
    return 1;
  }

  return 0;
}
