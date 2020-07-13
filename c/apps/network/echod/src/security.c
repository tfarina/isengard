#include "security.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ulog.h"

void security_check_daemon_user(char const *name) {
  struct passwd *pw;

  pw = getpwnam(name);
  if (pw == 0) {
    ulog_fatal("user '%s' not found", name);
  }
}

int drop_privileges(char const *username) {
  struct passwd *pw;
  gid_t gid;
  uid_t uid;
  int rc;

  pw = getpwnam(username);
  gid = pw->pw_gid;
  uid = pw->pw_uid;

  rc = setgid(gid);
  if (rc < 0) {
    ulog_error("unable to setgid() to %ld: %s", (long) gid, strerror(errno));
    return -1;
  }

  rc = setgroups(1, &gid);
  if (rc < 0) {
    ulog_error("setgroups(1, %ld) failed: %s", (long) gid, strerror(errno));
    return -1;
  }

  rc = setuid(uid);
  if (rc < 0) {
    ulog_error("unable to setuid() to %ld: %s", (long) uid, strerror(errno));
    return -1;
  }

  ulog_info("successfully dropped root privileges");

  return 0;
}
