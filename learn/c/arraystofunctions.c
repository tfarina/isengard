/*
 * "Passing arrays to functions
 *
 * When an array is passed to a function, it's passed as a pointer to the
 * first element, whether it's a stack-based or heap-based array. The pointer
 * contains no other size or type information. This behavior is called pointer
 * decay. When you pass an array to a function, you must always specify the
 * number of elements in a separate parameter. This behavior also implies that
 * the array elements aren't copied when the array gets passed to a function."
 *
 * 6.3.2.1    Lvalues, arrays, and function designators
 *
 * 3  Except when it is the operand of the sizeof operator, or the unary &
 * operator, or is a string literal used to initialize an array, an expression
 * that has type "array of type" is converted to an expression with type
 * "pointer to type" that points to the initial element of the array object
 * and is not an lvalue. If the array object has register storage class, the
 * behavior is undefined.
 *
 * Note: The important part above is the: "array of type" is converted to an
 * expression with type "pointer to type" that points to the initial element of
 * the array object.
 * In simple language, it is clearly saying that arrays are converted to
 * pointers when passed to functions.

 * Refs:
 * https://learn.microsoft.com/en-us/cpp/cpp/arrays-cpp?view=msvc-170
 * https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf
 */

#include <stdio.h>

#define ARRAY_SIZE 5

static
void
print_array(int *arr, size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      printf("%d", arr[i]);
    }
}

int
main(void)
{
  int numbers[ARRAY_SIZE] = { 1, 2, 3, 4, 5 };

  print_array(numbers, ARRAY_SIZE);

  return 0;
}
