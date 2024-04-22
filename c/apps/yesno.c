#include <stdio.h>

int
main(void)
{
  char answer;

  fputs("Are you sure you want to exit? (y, n) ", stdout);
  scanf("%c", &answer);

  fprintf(stdout, "%c\n", answer);

  return 0;
}
