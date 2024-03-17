#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os_path.h"
#include "xstring.h"

/*
 * To allocate memory for an array, just multiply the numbers of elements in
 * the array by the size of each array element (sizeof type).
 * https://www.embedded.com/allocating-arrays/

 * To allocate an array in the heap, use malloc, and compute the number of
 * bytes that are needed, using the multiplication described above.
 * http://cs.ecu.edu/karl/3300/spr16/Notes/C/Array/heap.html
 */

/*
 * [out] pulCount A pointer to a count of integer values pointed to by the
 *                ppNumbers parameter.
 * [out] ppNumbers A pointer to a pointer to the integer values.
 */
static int
parse_txt_file_v2(char const *filename, unsigned *pulCount, int **ppNumbers)
{
  FILE *fp;
  int ch;
  int numLines = 0;
  char linebuf[BUFSIZ];
  unsigned cValues;
  int *pIntNumbers;

  fp = fopen(filename, "r");
  if (NULL == fp)
  {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return -1;
  }

  /* Count the number of lines in the file */
  ch = fgetc(fp);
  while (ch != EOF)
  {
    if (ch == '\n')
      numLines++;

    ch = fgetc(fp);
  }

  printf("There are %d lines in this file.\n", numLines);

  /* Allocate a region of memory large enough for the array */
  cValues = 0;
  pIntNumbers = malloc(numLines * sizeof(int));
  if (NULL == pIntNumbers)
  {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  rewind(fp);

  /* Traverse the file line by line */
  while (fgets(linebuf, sizeof(linebuf), fp))
  {
    /* Assign a value to each element in the array */
    int number = atoi(linebuf);
    pIntNumbers[cValues] = number;
    cValues++;
  }

  *pulCount = cValues;
  *ppNumbers = pIntNumbers;

  fclose(fp);

  return 0;
}

int
main(int argc, char **argv)
{
  int retval;
  char *filename;
  unsigned uCount;
  int *pNumbers;
  unsigned i;

  if (argc != 2)
  {
    fputs("usage: numbersv2 numbers.txt\n", stderr);
    return 1;
  }

  filename = xstrdup(argv[1]);

  if (!os_path_exists(filename))
  {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return 1;
  }

  retval = parse_txt_file_v2(filename, &uCount, &pNumbers);
  if (retval < 0)
  {
    return 1;
  }

  for (i = 0; i < uCount; i++)
  {
    printf("%d\n", pNumbers[i]);
  }

  free(filename);

  return 0;
}
