#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "xstring.h"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ch;
  int numLines = 0;

  if (argc != 2) {
    fputs("usage: countlines FILE\n", stderr);
    return 1;
  }

  filename = xstrdup(argv[1]);

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  ch = fgetc(fp);
  while (ch != EOF)
  {
    if (ch == '\n')
      numLines++;

    ch = fgetc(fp);
  }

  fclose(fp);

  printf("There are %d lines in %s file.\n", numLines, filename);

  return 0;
}
