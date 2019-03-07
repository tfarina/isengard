#include "futils.h"

#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Gets the current user's home directory.
 */
const char *f_get_home_dir(void)
{
  return getenv("HOME");
}

/**
 * Gets the user name of the current user.
 */
const char *f_get_user_name(void)
{
  struct passwd *pw;

  pw = getpwuid(geteuid());

  return pw->pw_name;
}
