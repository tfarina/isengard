#include <stdio.h>

int
main(void)
{
  int n = 1;

label:
  printf("%d ", n);
  n++;
  if (n <= 10) {
    goto label;
  }

  putc('\n', stdout);

  return 0;
}
