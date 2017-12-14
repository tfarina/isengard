#include <stdio.h>

/*
 * https://denniskubes.com/2012/08/19/pointers-and-arrays-in-c/
 *
 * FACT: Arrays are NOT pointers and pointers ARE NOT arrays.
 */

int main(int argc, char **argv) {
  int numbers[5] = {1, 2, 3, 4, 5};

  /* FIRST: Using standard array notation. */
  int *ptr1 = numbers;
  int val1 = numbers[0];

  /* SECOND: Using address-of array notation. */
  int *ptr2 = &numbers[0];
  int val2 = *(&numbers[0]);

  printf("address of ptr1 -> %p\n", (void *)ptr1);
  printf("address of ptr2 -> %p\n", (void *)ptr2);

  printf("%d\n", val1);
  printf("%d\n", val2);

  return 0;
}
