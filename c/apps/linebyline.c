#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  char *filename;
  struct stat st;
  FILE *fp;
  char linebuf[BUFSIZ];
  int lineno = 0;

  if (argc != 2) {
    fprintf(stderr, "usage: linebyline <filename>\n");
    return 1;
  }

  filename = argv[1];

  if (stat(filename, &st)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
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
