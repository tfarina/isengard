#include "user.h"

#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "os_path.h"

/**
 * Returns the current user's home directory.
 */
char *
user_home_dir(void)
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

/**
 * Returns the base directory for user-specific configuration files.
 */
char *
user_config_dir(void)
{
  char *homedir;
  char *configdir;

  homedir = user_home_dir();
  configdir = os_path_join(homedir, ".config");

  return configdir;
}
