#include <stdio.h>

int main(int argc, char **argv) {
  /*
   * Ref:
   * http://denniskubes.com/2012/08/14/do-you-know-what-p-does-in-c/
   *
   * C Postfix Increment and Decrement Operators
   * https://learn.microsoft.com/en-us/cpp/c-language/c-postfix-increment-and-decrement-operators?view=msvc-170
   *
   * Postfix Increment and Decrement Operators: ++ and --
   * https://learn.microsoft.com/en-us/cpp/cpp/postfix-increment-and-decrement-operators-increment-and-decrement?view=msvc-170
   */

  int x = 0;
  int y = x++;

  printf("x = %d, y = %d\n", x, y);

  /*
   * This is what is going on behind the scenes when you use postfix increment operator (++).
   */
  x = 0;
  y = x;
  x = x + 1;

  printf("x = %d, y = %d\n", x, y);

  return 0;
}
