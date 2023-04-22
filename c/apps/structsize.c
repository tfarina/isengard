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

  printf("size of structure in BYTES: %d\n", sizeof(struct vector));
  printf("size of structure in BYTES: %d\n", sizeof(v1));

  return 0;
}
