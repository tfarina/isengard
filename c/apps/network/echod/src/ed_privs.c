#include "ed_privs.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ed_globals.h"
#include "ulog.h"

void ed_privs_check_daemon_user(char const *user) {
  struct passwd *pw;

  pw = getpwnam(user);
  if (pw == 0) {
    ulog_fatal("user '%s' not found", user);
  }

  ed_g_daemon_uid = pw->pw_uid;
  ed_g_daemon_gid = pw->pw_gid;
}

int ed_privs_drop(uid_t uid, gid_t gid) {
  int rc;

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
