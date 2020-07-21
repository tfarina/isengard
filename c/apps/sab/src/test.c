#include "test.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int test_exists(char const *path)
{
  struct stat sb;
  int rv;

  rv = stat(path, &sb);

  return rv == 0;
}

int test_isdir(char const *path)
{
  struct stat sb;
  int rc;

  rc = stat(path, &sb);

  return rc == 0 && S_ISDIR(sb.st_mode);
}
