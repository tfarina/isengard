#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os_path.h"
#include "xstring.h"

static unsigned count; /* count of numbers */
static int *numbers; /* array of numbers */

static int
parse_txt_file(char const *filename)
{
  FILE *fp;
  char linebuf[BUFSIZ];

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  count = 100;
  numbers = malloc(sizeof(int) * count);
  if (numbers == NULL)
  {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  count = 0;
  /* Traverse the file line by line */
  while (fgets(linebuf, sizeof(linebuf), fp))
  {
    int number = atoi(linebuf);
    numbers[count] = number;
    count++;
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
    fputs("usage: numbers.txt\n", stderr);
    return 1;
  }

  filename = xstrdup(argv[1]);

  if (!os_path_exists(filename)) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = parse_txt_file(filename);
  if (retval < 0) {
    return 1;
  }

  for (i = 0; i < count; i++)
  {
    printf("%d\n", numbers[i]);
  }

  free(filename);

  return 0;
}
