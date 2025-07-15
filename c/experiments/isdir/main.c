#include <stdio.h>

#include "os_path.h"

int main(int argc, char **argv)
{
  if (os_path_isdir("notadirectory.txt")) {
    fputs("notadirectory.txt is directory\n", stdout);
  }

  if (os_path_isdir("/usr/bin")) {
    fputs("/usr/bin is a directory\n", stdout);
  }

  return 0;
}
