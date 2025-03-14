#include <stdio.h>

int main(void)
{
  /*
   * Ref:
   *
   * Subscript Operator []
   * https://learn.microsoft.com/en-us/cpp/cpp/subscript-operator?view=msvc-170

   * 6.5.2.1 Array subscripting
   *
   * Constraints
   * One of the expressions shall have type "pointer to complete object type",
   * the other expression shall have integer type, and the result has type
   * "type".
   *
   * Semantics
   * A postfix expression followed by an expression in square brackets[]is a
   * subscripted designation ofan element of an array object. The definition
   * of the subscript operator[] is that E1[E2] is identical to(*((E1)+(E2))).
   * Because of the conversion rules that apply to the binary+ operator, if E1
   * is an array object (equivalently, a pointer to the initial element of an
   * array object) and E2 is an integer, E1[E2] designates the E2-th element
   * of E1 (counting from zero).
   *
   * https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf
   */

  int arr[5] = {1, 2, 3, 4, 5};

  /*
   * This example demonstrates the "counting from zero" in the sentence
   * above. It retrieves the first element in the array.
   */
  printf("%d\n", arr[0]);

  /*
   * The next two examples shows that the expression arr[2] is identical
   * to 2[arr]. The reason for that comes from the subscript operator []
   * definition.
   */
  printf("%d\n", arr[2]);

  printf("%d\n", 2[arr]);

  /*
   * The definition of the subscript operator [] is that E1[E2] (and by E1 we
   * mean Expression 1 and E2 Expression 2) is identical to (*((E1) + (E2))).
   *
   * https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf
   *
   * The following demonstrates that.
   */
  printf("%d\n", *(arr + 2));

  printf("%d\n", *(2 + arr));

  /*
   * In examples below we demonstrate the three ways of writing E1[0] using
   * the pointer syntax to show they are all equivalent to E1[0] and yield
   * the same result.
   */
  printf("%d\n", *(arr + 0));
  printf("%d\n", *(0 + arr));
  printf("%d\n", *arr);

  return 0;
}
