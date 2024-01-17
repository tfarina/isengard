/*
 * The code below was taken from https://www.lemoda.net/c/array-initialization/index.html
 *
 * References:
 * https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html#Initializing-Arrays
 * https://en.cppreference.com/w/c/language/array_initialization
 *
 * C99 standard (ISO/IEC 9899:1999): 6.7.8/12-38 Initialization (p: 126-130)
 * C89/C90 standard (ISO/IEC 9899:1990): 6.7.9 Initialization
 * https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf#subsection.6.7.9
 *
 */

#include <stdio.h>

#define ARRAY_SIZE 5

int
main(void)
{
  /* Initialize all the elements. */
  int array_all[ARRAY_SIZE] = { 1, 2, 3, 4, 5 };
  /* If only the first elements are initialized, the remaining
     elements are all set to zero. */
  int array_first_two[ARRAY_SIZE] = { 1, 2 };
  /* So, if the first element is set to zero, then everything else
     is, too. */
  int array_zero[ARRAY_SIZE] = { 0 };
  /* But if nothing is initialized, the whole array contains random
     junk. */
  int array_not_static[ARRAY_SIZE];
  /* Unless the array is declared using "static", in which case it
     is zeroed. */
  static int array_static[ARRAY_SIZE];
  int i;

  for (i = 0; i < ARRAY_SIZE; i++) {
    printf("%d: %d %d %d %d %d\n",
	   i,
	   array_all[i],
	   array_first_two[i],
	   array_zero[i],
	   array_not_static[i],
	   array_static[i]);
  }

  return 0;
}
