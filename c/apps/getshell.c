#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  char const *shell;

  shell = getenv("SHELL");

  if (shell == NULL || *shell == '\0') {
    shell = "/bin/sh";
  }

  printf("%s\n", shell);

  return 0;
}
