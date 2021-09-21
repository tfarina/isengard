#include "os.h"

#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "os_path.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif

char *
os_hostname(void)
{
  static char buf[MAXHOSTNAMELEN + 1];

  if (gethostname(buf, sizeof(buf)) < 0) {
    fprintf(stderr, "gethostname() failed: %s\n", strerror(errno));
    return (char *) 0;
  }

  buf[MAXHOSTNAMELEN] = '\0'; /* Null terminate, just to be safe. */

  return buf;
}

char *
os_homedir(void)
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

char *
os_userconfigdir(void)
{
  char *homedir;
  char *configdir;

  homedir = os_homedir();
  configdir = os_path_join(homedir, ".config");

  return configdir;
}

char *
os_username(void)
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
