#include <stdio.h>
#include <stdlib.h>

/*
 * <code>
 * int *p = NULL;
 * DoSomething(&p);
 * </code>
 *
 * Passing &p to DoSomething function, is equivalent to a normal assignment:
 *
 * <code>
 * int **pp = &p;
 * </code>
 */
int
DoSomething(int **ppInt)
/* int **ppInt = &p; stores the address of a pointer variable */
{
  int *pInt;

  /* Allocate memory for an integer */
  pInt = malloc(sizeof(int));
  if (NULL == pInt)
  {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  /* Assign a value to it */
  *pInt = 10;

  *ppInt = pInt; /* changes the address of p variable to where pInt points to, so it can reach its value */

  return 0;
}

int main(void)
{
  /*
   * Ref:
   * Address-of operator: &
   * https://learn.microsoft.com/en-us/cpp/cpp/address-of-operator-amp?view=msvc-170
   *
   * Indirection and address-of operators
   * https://learn.microsoft.com/en-us/cpp/c-language/indirection-and-address-of-operators?view=msvc-170
   *
   * Pointer Declarations
   * https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170
   *
   * C – Pointer to Pointer (Double Pointer)
   * https://www.geeksforgeeks.org/c-pointer-to-pointer-double-pointer/
   *
   * Address of a pointer is equal to a pointer to pointer?
   * https://stackoverflow.com/questions/17099498/address-of-a-pointer-is-equal-to-a-pointer-to-pointer
   *
   * Double pointers in C
   * https://joequery.me/notes/double-pointers-c/
   *
   * C Double Pointers
   * https://lloydrochester.com/post/c/double-pointer/
   */

  int rc = 0;
  int *p = NULL;

  /* If we pass NULL here, or don't pass &p (the address-of p), then the
   * pointer to pointer (double pointer) ppInt will be left uninitialized
   * and a segmentation fault error will occur. */
  rc = DoSomething(&p);
  if (rc < 0)
  {
    return 1;
  }

  printf("The value of p is: %d\n", *p);

  return 0;
}
