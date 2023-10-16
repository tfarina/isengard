#include <stdio.h>

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
   * Double pointers in C
   * https://joequery.me/notes/double-pointers-c/
   *
   * C Double Pointers
   * https://lloydrochester.com/post/c/double-pointer/
   */

  int x = 10;
  int *p = &x;
  int **pp = &p;

  /*
   * This shows that the unary address-of operator (&) returns the address of
   * (that is, a pointer to) its operand.
   */
  printf("The address of p: %p\n", &p);

  /*
   * This shows that a variable declared as a pointer to pointer holds the
   * address of another pointer variable.
   */
  printf("The address of p: %p\n", pp);

  return 0;
}
