#include <stdio.h>

int
main(void)
{
  char yes_or_no;

  fputs("Are you sure you want to exit? (y, n) ", stdout);

  scanf("%c", &yes_or_no);

  fprintf(stdout, "%c\n", yes_or_no);

  return 0;
}
