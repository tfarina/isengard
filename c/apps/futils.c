#include "futils.h"

#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Gets the current user's home directory.
 */
char *f_get_home_dir(void)
{
  char *home;
  struct passwd *pw;

  home = getenv("HOME");

  if ((home == NULL) || (*home == '\0')) {
    pw = getpwuid(getuid());
    if (pw != NULL && pw->pw_dir != NULL && *pw->pw_dir != '\0') {
      home = pw->pw_dir;
    } else {
      home = "/";
    }
  }

  return home;
}

/**
 * Gets the user name of the current user.
 */
char *f_get_user_name(void)
{
  struct passwd *pw;

  pw = getpwuid(geteuid());

  return pw->pw_name;
}
