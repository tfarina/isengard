#include <stdio.h>

/* http://denniskubes.com/2012/08/14/do-you-know-what-p-does-in-c/ */

int main(int argc, char **argv) {
  int x = 0;
  int y = x++;

  printf("x = %d, y = %d\n", x, y);

  /* This is what is going on behind the scenes when you use postfix operator (++). */
  x = 0;
  y = x;
  x = x + 1;

  printf("x = %d, y = %d\n", x, y);

  return 0;
}
