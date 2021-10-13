#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrutils.h"
#include "test.h"

static char **symbols; /* array of pointers to symbols */
static unsigned nb_symbols; /* number of symbols */

static int
parse_txt_file(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];
  char *market_type;
  char *symbol;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  market_type = NULL;
  symbol = NULL;

  /* Loops through the file reading line by line. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    market_type = str_substring(linebuf, 108, 111);

    if (str_startswith(market_type, "010")) {
      symbol = str_substring(linebuf, 2, 14);
      symbol = str_strip(symbol);

      /* This is a small trick.
       * In the first pass 'companies' will be NULL, then this realloc call
       * will be equivalent to malloc(sizeof(company_t) * 1). */
      symbols = realloc(symbols, sizeof(char *) * nb_symbols + 1);
      if (symbols == 0) {
	fputs("Out of memory\n", stderr);
        break;
      }

      symbols[nb_symbols++] = symbol;
    }
  }

  fclose(fp);

  return 0;
}

int
main(int argc, char **argv)
{
  int retval;
  char *filename;
  unsigned i;

  if (argc != 2) {
    fputs("usage: b3security TITULOS_NEGOCIAVEIS.txt\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);

  if (!test_exists(filename)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = parse_txt_file(filename);
  if (retval < 0) {
    return 1;
  }

  for (i = 0; i < nb_symbols; i++) {
    printf("%s\n", symbols[i]);
  }

  free(filename);

  return 0;
}
