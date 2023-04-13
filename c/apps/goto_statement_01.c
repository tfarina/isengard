#include <stdio.h>

int
main(void)
{
  int n = 1;

loop_restart:
  printf("%d ", n);
  n++;
  if (n <= 10) {
    goto loop_restart;
  }

  putc('\n', stdout);

  return 0;
}
