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
  int **pp = &p;

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

  /*
   * If we use the unary indirection operator (*) on a pointer to pointer
   * variable, that is it, if we dereference a pointer to pointer or a double
   * pointer, what do we get? We get the VALUE of the pointer we are pointing
   * to, and that is the address of x variable!
   * In both cases it returns the value, when used on a pointer it returns the
   * value of POD (Plain Old Data), but when used on a pointer to pointer it
   * returns the ADDRESS!
   */
  printf("The address of x: %p\n", p);
  printf("The address of x via indirection: %p\n", *pp);

  return 0;
}
