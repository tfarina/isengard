#include "id.h"

#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#define ROOT_UID 0

int id_is_root(void) {
  if (getuid() == ROOT_UID) {
    return 1;
  }

  return 0;
}

char const *id_get_username(void) {
  uid_t uid;
  struct passwd *pw;

  uid = getuid();

  pw = getpwuid(uid);
  if (pw == 0) {
    return "unknown";
  }

  return pw->pw_name;
}

char const *id_get_groupname(void) {
  gid_t gid;
  struct group *gr;

  gid = getgid();

  gr = getgrgid(gid);
  if (gr == 0) {
    return "unknown";
  }

  return gr->gr_name;
}
