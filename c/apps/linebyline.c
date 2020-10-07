#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  FILE *fp;
  char linebuf[BUFSIZ];
  int lineno = 0;
  struct stat st;

  if (argc != 2) {
    fprintf(stderr, "usage: linebyline <filename>\n");
    exit(EXIT_FAILURE);
  }

  if (stat(argv[1], &st)) {
    fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* This loops through the lines of the file, printing it. */
  while (fgets(linebuf, sizeof(linebuf), fp)) {
    lineno++;
    printf("%d:%s", lineno, linebuf);

    /* This loops through *every* character present in this line. */
    char *c;
    for (c = linebuf; *c; c++) {
      printf("%c", *c);
    }
  }

  fclose(fp);

  return 0;
}
