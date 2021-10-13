#include <stdio.h>
#include <stddef.h>

struct vector {
  char *data;
  size_t datasize;
  size_t capacity;
  size_t size;
};

int
main(void)
{
  struct vector v1;

  printf("The size of vec structure is %d\n", sizeof(struct vector));
  printf("The size of vec structure is %d\n", sizeof(v1));

  return 0;
}
