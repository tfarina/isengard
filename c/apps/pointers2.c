#include <stdio.h>

/* https://denniskubes.com/2012/08/17/basics-of-memory-addresses-in-c/ */

int main(int argc, char **argv) {
  char ch = 'a';
  char *cptr = &ch;

  printf("The memory address of 'ch' is -> %p\n", (void *)&ch);
  printf("The value of the pointer 'cptr' is -> %p\n", (void *)cptr);
  printf("The memory address of the pointer 'cptr' is -> %p\n", (void *)&cptr);

  return 0;
}
