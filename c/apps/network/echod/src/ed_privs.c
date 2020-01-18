#include "ed_privs.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ed_globals.h"
#include "ed_log.h"
#include "ed_rcode.h"

void ed_privs_check_owner(char const *user) {
  struct passwd *pw;

  pw = getpwnam(user);
  if (pw == 0) {
    ed_log_fatal("user '%s' not found", user);
  }

  ed_g_owner_uid = pw->pw_uid;
  ed_g_owner_gid = pw->pw_gid;
}

int ed_privs_drop(uid_t uid, gid_t gid) {
  int rc;

  rc = setgid(gid);
  if (rc < 0) {
    ed_log_error("unable to set gid to %d: %s", gid, strerror(errno));
    return ED_ERROR;
  }

  rc = setgroups(1, &gid);
  if (rc < 0) {
     ed_log_error("unable to set setgroups() with gid %d", gid);
     return ED_ERROR;
  }

  rc = setuid(uid);
  if (rc < 0) {
    ed_log_error("unable to set uid to %d: %s", uid, strerror(errno));
    return ED_ERROR;
  }

  ed_log_info("successfully dropped root privileges");

  return ED_OK;
}
