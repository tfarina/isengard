#include <stdio.h>

#include "test.h"

int main(int argc, char **argv)
{
  if (test_isdir("notadirectory.txt")) {
    fputs("notadirectory.txt is directory\n", stdout);
  }

  if (test_isdir("/usr/bin")) {
    fputs("/usr/bin is a directory\n", stdout);
  }

  return 0;
}
