#include <stdio.h>

/* http://denniskubes.com/2012/08/14/do-you-know-what-p-does-in-c/ */

int main(int argc, char **argv) {
  int myarray[4] = {1, 2, 3, 0};
  int *p = myarray;
  int out = 0;

  while (out = *p++) {
    printf("%d ", out);
  }

  return 0;
}
