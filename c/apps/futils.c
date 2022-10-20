#include "futils.h"

#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

#include "ffileutils.h"

/**
 * Gets the current user's home directory.
 */
char *f_get_home_dir(void)
{
  char *home;
  struct passwd *pw;

  home = getenv("HOME");

  if ((home == NULL) || (*home == '\0')) {
    /*
     * Try to get user's home directory from the password file (/etc/passwd).
     */
    pw = getpwuid(getuid());
    if (pw != NULL && pw->pw_dir != NULL && *pw->pw_dir != '\0') {
      home = pw->pw_dir;
    } else {
      home = "/";
    }
  }

  return home;
}
