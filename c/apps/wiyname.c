#include <stdio.h>
#include <string.h>

/* http://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html */

int
main(void)
{
  char name[40];

  printf("What's your name? ");

  if (fgets(name, 40, stdin) == NULL) {
    return 1;
  }

  name[strcspn(name, "\n")] = 0;
  printf("Hello %s\n", name);

  return 0;
}
