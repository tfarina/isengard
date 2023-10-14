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
   */

  int x = 10;
  int *p = &x;

  /*
   * This shows that the unary address-of operator (&) returns the address of
   * (that is, a pointer to) its operand.
   */
  printf("The address of x: %p\n", &x);

  /*
   * This shows that a variable declared as a pointer holds a memory address.
   */
  printf("The address of x: %p\n", p);

  return 0;
}
