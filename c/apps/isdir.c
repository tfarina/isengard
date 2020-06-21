#include <stdio.h>
#include <stdlib.h>

#include "test.h"

int main(int argc, char **argv)
{
  if (test_isdir("notadirectory.txt")) {
    printf("notadirectory.txt is directory\n");
  }

  if (test_isdir("/usr/bin")) {
    printf("/usr/bin is a directory\n");
  }

  return 0;
}
