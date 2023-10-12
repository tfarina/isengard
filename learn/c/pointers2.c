#include <stdio.h>

/* https://denniskubes.com/2012/08/17/basics-of-memory-addresses-in-c/ */

int main(int argc, char **argv) {
  char ch = 'a';
  char *cptr = &ch;

  printf("The memory address of ch is: %p\n", &ch);
  printf("The value of the pointer cptr is: %p\n", cptr);
  printf("The memory address of the pointer cptr is: %p\n", &cptr);
  printf("The value of ch is: %c\n", *cptr);

  return 0;
}
