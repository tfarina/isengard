#include "futils.h"

#include <stdlib.h>

/**
 * Gets the current user's home directory.
 */
const char *f_get_home_dir(void)
{
  return getenv("HOME");
}
