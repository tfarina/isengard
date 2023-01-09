#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xstring.h"

int main(int argc, char **argv) {
  char buffer[] = "This is the buffer text.";
  char *newstr;

  printf("Original: %s\n", buffer);

  newstr = xstrdup(buffer);
  if (newstr)
  {
    printf("Copy: %s\n", newstr);
    free(newstr);
  }

  return 0;
}
