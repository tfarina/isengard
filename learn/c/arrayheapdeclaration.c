#include <stdio.h>
#include <stdlib.h>

/*
 * Heap Declarations
 *
 * Sometimes you don't know the size of an array at compile time, other it may
 * be too large to allocate on the stack. In these situations, you can allocate
 * an array on the heap, by using malloc or calloc. They return a pointer to
 * the first element allocated. You can then use the subscript operator [] on
 * the pointer variable the same way you would use on stack-based array. It is
 * also possible to use pointer arithmetic to move the pointer to any arbitrary
 * elements in the the array.
 *
 * Refs:
 * https://learn.microsoft.com/en-us/cpp/cpp/arrays-cpp?view=msvc-170
 * https://www.embedded.com/allocating-arrays/
 * http://cs.ecu.edu/karl/3300/spr16/Notes/C/Array/heap.html
 *
 */

#define NUM_ELEMENTS 10

int
main(void)
{
  int *int_ptr;
  unsigned i;

  /*
   * To allocate memory for an array on the heap, use malloc and compute
   * the number of bytes needed by multiplying the number of elements
   * by the size of each element.
   */
  int_ptr = malloc(NUM_ELEMENTS * sizeof(int));
  if (NULL == int_ptr)
  {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Assign a value to each element */
  for (i = 0; i < NUM_ELEMENTS; i++)
  {
    int_ptr[i] = i + 1;
  }

  /* Access each element with subscript operator [] */
  for (i = 0; i < NUM_ELEMENTS; i++)
  {
    printf("%d ", int_ptr[i]);
  }

  /* Don't forget to release the memory */
  free(int_ptr);

  return 0;
}
