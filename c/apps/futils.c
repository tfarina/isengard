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
 * Gets the user's configuration directory, where information like
 * user preferences and settings can be stored.
 */
char *f_get_user_config_dir(void)
{
  char *homedir;
  char *configdir;

  homedir = f_get_home_dir();
  configdir = f_build_filename(homedir, ".config");

  return configdir;
}

/**
 * Gets the user name of the current user.
 */
char *f_get_user_name(void)
{
  uid_t uid;
  struct passwd *pw;

  uid = getuid();
  pw = getpwuid(uid);
  if (pw == 0) {
    return 0;
  }

  return pw->pw_name;
}
