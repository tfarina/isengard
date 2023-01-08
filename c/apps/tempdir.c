#include "tempdir.h"

#include <stdlib.h>
#include <unistd.h>

#include "xstring.h"
#include "test.h"

/*
 * Code borrowed from:
 *
 * https://github.com/stephenmathieson/tempdir.c/blob/master/tempdir.c
 *
 * Modified to remove stuff related to Windows port.
 */

static char const *env_vars[] = {
  "TMPDIR",
  "TEMP",
  "TMP",
  NULL
};

static const char *platform_dirs[] = {
  "/tmp",
  "/var/tmp",
  "/usr/tmp",
  NULL
};

/**
 * Searches through the following list of directories:
 *
 *  1. The directory named by the TMPDIR environment variable.
 *  2. The directory named by the TEMP environment variable.
 *  3. The directory named by the TMP environment variable.
 *
 *  4. A platform-specific location:
 *
 *       On Unix/Linux, the directories /tmp, /var/tmp, and /usr/tmp, in that order.
 *
 *  5. As a last resort, the current working directory.
 */
char *
f_tempdir(void)
{
  size_t i;
  char cwd[256];

  for (i = 0; *(env_vars + i); i++) {
    char const *dir = getenv(*(env_vars + i));

    if (dir != NULL && test_isdir(dir)) {
      return xstrdup(dir);
    }
  }

  for (i = 0; *(platform_dirs + i); i++) {
    char const *dir = *(platform_dirs + i);

    if (test_isdir(dir)) {
      return xstrdup(dir);
    }
  }

  /* As a last resort, fallback to cwd. */
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    return xstrdup(cwd);
  }

  return NULL;
}
