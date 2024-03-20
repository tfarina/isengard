#include <stdio.h>

static
int /* BOOL */
prompt_user(char *str)
{
  char ch;

  printf("%s", str);

  ch = getchar();
  if (ch == 'y' || ch == 'Y')
  {
    return 1; /* TRUE */
  }

  return 0; /* FALSE */
}

int
main(void)
{
  if (prompt_user("Are you sure you want to permanently delete the registry key? (Y/N) "))
  {
    fputs("Key deleted\n", stdout);
  }
  else
  {
    fputs("Operation cancelled\n", stdout);
  }

  return 0;
}
