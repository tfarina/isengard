#include "ed_privs.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ed_globals.h"
#include "ed_log.h"
#include "ed_rcode.h"

void ed_privs_check_owner(char const *username) {
  struct passwd *pw;

  pw = getpwnam(username);
  if (pw == 0) {
    ed_log_fatal("user '%s' not found", username);
  }

  ed_g_owner_uid = pw->pw_uid;
  ed_g_owner_gid = pw->pw_gid;
}

int ed_privs_drop(char const *username) {
  struct passwd *pw;
  gid_t runas_gid;
  uid_t runas_uid;
  int rc;

  pw = getpwnam(username);
  if (pw == NULL) {
    ed_log_error("user '%s' not found", username);
    return ED_ERROR;
  }

  runas_gid = pw->pw_gid;
  runas_uid = pw->pw_uid;

  rc = setgid(runas_gid);
  if (rc < 0) {
    ed_log_error("unable to set gid to %d: %s", runas_gid, strerror(errno));
    return ED_ERROR;
  }

  rc = initgroups(username, runas_gid);
  if (rc < 0) {
     ed_log_error("unable to set initgroups() with gid %d", runas_gid);
     return ED_ERROR;
  }

  rc = setuid(runas_uid);
  if (rc < 0) {
    ed_log_error("unable to set uid to %d: %s", runas_uid, strerror(errno));
    return ED_ERROR;
  }

  ed_log_info("successfully dropped root privileges");

  return ED_OK;
}
