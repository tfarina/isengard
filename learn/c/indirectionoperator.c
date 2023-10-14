#include <stdio.h>

int main(void)
{
  /*
   * Ref:
   * Indirection Operator: *
   * https://learn.microsoft.com/en-us/cpp/cpp/indirection-operator-star?view=msvc-170
   *
   * Indirection and address-of operators
   * https://learn.microsoft.com/en-us/cpp/c-language/indirection-and-address-of-operators?view=msvc-170J
   */

  int x = 10;
  int *p = &x;

  printf("The direct value of x: %d\n", x);

  /*
   * This shows that the unary indirection operator (*) accesses a
   * value indirectly, through a pointer, dereferencing it.
   * The operand must be a pointer type. The result of the operation
   * is the value addressed by the operand; that is, the value at the
   * address to which its operand points. The type of the result is the
   * type that the operand addresses.
   */
  printf("The indirect value of x: %d\n", *p);
}
