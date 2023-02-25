#include <stdio.h>
#include <string.h>

/* http://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html */

int
main(void)
{
  char name[40];

  fputs("What's your name? ", stdout);

  if (fgets(name, sizeof(name), stdin) == NULL) {
    return 1;
  }

  name[strcspn(name, "\n")] = 0;
  printf("Hello %s\n", name);

  return 0;
}
