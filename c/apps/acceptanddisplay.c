#include <stdio.h>

int
main(void)
{
  char first_name[1024];

  fputs("First Name: ", stdout);
  fgets(first_name, sizeof(first_name), stdin);

  fprintf(stdout, "Name is: %s", first_name);

  return 0;
}
