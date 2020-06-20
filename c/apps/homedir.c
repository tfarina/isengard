#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "futils.h"

static int test_isdir(char const *path)
{
  struct stat sb;
  int rc;

  rc = stat(path, &sb);

  return rc == 0 && S_ISDIR(sb.st_mode);
}

int main(int argc, char **argv) {
  const char *homedir;

  homedir = f_get_home_dir();

  printf("%s\n", homedir);

  if (test_isdir(homedir)) {
    printf("Looks look %s is a directory\n", homedir);
  }

  return 0;
}
