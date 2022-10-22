#include "security.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ulog.h"

int drop_privileges(char const *username) {
  struct passwd *pw;
  gid_t gid;
  uid_t uid;
  int rc;

  pw = getpwnam(username);
  if (pw == 0) {
    ulog_error("unable to get user id for %s", username);
    return -1;
  }
  gid = pw->pw_gid;
  uid = pw->pw_uid;

  /**
   * Be sure to pare down the ancillary groups for the process before doing
   * anything else because the setgroups() system call requires root privileges.
   * https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch01s03.html
   */
  rc = setgroups(1, &gid);
  if (rc < 0) {
    ulog_error("setgroups(1, %ld) failed: %s", (long) gid, strerror(errno));
    return -1;
  }

  rc = setgid(gid);
  if (rc < 0) {
    ulog_error("unable to setgid() to %ld: %s", (long) gid, strerror(errno));
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
