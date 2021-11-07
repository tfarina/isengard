#include <stdint.h>
#include <stdio.h>

int
main(void)
{
  /* SIZE_MAX or (size_t) -1 is the limit of 'size_t' type. */
  printf("%zu\n", SIZE_MAX);
  printf("%zu\n", (size_t) -1);

  return 0;
}
