#include "ed_privs.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ed_log.h"
#include "ed_rcode.h"

int ed_change_user(char const *username) {
  struct passwd *pw;
  gid_t gid;
  uid_t uid;

  pw = getpwnam(username);
  if (pw == NULL) {
    ed_log_error("cannot find user '%s' to switch to", username);
    return ED_ERROR;
  }

  gid = pw->pw_gid;
  uid = pw->pw_uid;

  if (setgroups(1, &gid) != 0) {
    ed_log_error("setgroups failed");
    return ED_ERROR;
  }

  if (setresgid(gid, gid, gid) != 0) {
    ed_log_error("setting group id to user '%s' failed: %s\n",
                 username, strerror(errno));
    return ED_ERROR;
  }

  if (setresuid(uid, uid, uid) != 0) {
    ed_log_error("%s: setting user id to user '%s' failed: %s\n",
                 username, strerror(errno));
    return ED_ERROR;
  }

  return ED_OK;
}
