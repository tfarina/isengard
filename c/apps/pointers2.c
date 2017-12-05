#include <stdio.h>

/* https://denniskubes.com/2012/08/17/basics-of-memory-addresses-in-c/ */

int main(int argc, char **argv) {
  char ch = 'a';

  printf("address of ch is: %p\n", (void *)(&ch));

  return 0;
}
