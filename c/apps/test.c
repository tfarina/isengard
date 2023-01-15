#include "test.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Determines whether the given path (such as a file or directory) exists.
 *
 * @param[in] path A pointer to a string that contains the path to verify.
 *
 * [-e FILE] FILE exists?
 *
 * @return Returns true if the path exists; otherwise, false.
 */
int test_exists(char const *path)
{
  struct stat sb;
  int rv;

  rv = stat(path, &sb);

  return rv == 0;
}

/**
 * Verifies that the given path is a directory.
 *
 * @param[in] path A pointer to a string that contains the path to verify.
 *
 * [-d FILE ] FILE exists and is a directory?
 *
 * @return Returns true if path is a directory; otherwise, false.
 */
int test_isdir(char const *path)
{
  struct stat sb;
  int rc;

  rc = stat(path, &sb);

  return rc == 0 && S_ISDIR(sb.st_mode);
}
