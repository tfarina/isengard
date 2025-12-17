#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os_path.h"
#include "xstring.h"

typedef struct _INTARRAY
{
  unsigned cValues; /* Count of values in the array specified by the pNumbers member. */
  int *pNumbers; /* Pointer to an array of integer values. */
} INTARRAY, *PINTARRAY;

static int
parse_txt_file_v3(char const *filename, INTARRAY **ppIntArray)
{
  FILE *fp;
  int ch;
  int numLines = 0;
  char linebuf[BUFSIZ];
  INTARRAY *pIntArray = NULL;

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

  printf("There are %d lines in this file.\n", numLines);

  pIntArray = malloc(sizeof(INTARRAY));
  if (NULL == pIntArray)
  {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  pIntArray->cValues = 0;
  pIntArray->pNumbers = malloc(numLines * sizeof(int));
  if (NULL == pIntArray->pNumbers)
  {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  rewind(fp);

  /* Traverse the file line by line */
  while (fgets(linebuf, sizeof(linebuf), fp))
  {
    int number = atoi(linebuf);
    pIntArray->pNumbers[pIntArray->cValues] = number;
    pIntArray->cValues++;
  }

  *ppIntArray = pIntArray;

  fclose(fp);

  return 0;
}

int
main(int argc, char **argv)
{
  int retval;
  char *filename;
  unsigned i;
  INTARRAY *pIntArray = NULL;

  if (argc != 2)
  {
    fputs("usage: numbersv3 numbers.txt\n", stderr);
    return 1;
  }

  filename = xstrdup(argv[1]);

  if (!os_path_exists(filename))
  {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = parse_txt_file_v3(filename, &pIntArray);
  if (retval < 0)
  {
    return 1;
  }

  for (i = 0; i < pIntArray->cValues; i++)
  {
    printf("%d\n", pIntArray->pNumbers[i]);
  }

  free(filename);

  return 0;
}
