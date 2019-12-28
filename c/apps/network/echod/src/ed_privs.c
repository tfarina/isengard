#include "ed_privs.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "ed_log.h"
#include "ed_rcode.h"

uid_t ed_privs_get_uid(char const *username) {
  struct passwd *pw;

  if (username == NULL) {
    return 0;
  }

  pw = getpwnam(username);
  if (pw == NULL) {
    ed_log_error("user '%s' not found", username);
    return -1;
  }

  return pw->pw_uid;
}

gid_t ed_privs_get_gid(char const *username) {
  struct group *gr;

  if (username == NULL) {
    return 0;
  }

  gr = getgrnam(username);
  if (gr == NULL) {
    ed_log_error("group '%s' not found", username);
    return -1;
  }

  return gr->gr_gid;
}

int ed_change_user(char const *username) {
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

  rc = initgroups(username, runas_gid);
  if (rc < 0) {
     ed_log_error("unable to set initgroups() with gid %d", runas_gid);
     return ED_ERROR;
  }

  rc = setgid(runas_gid);
  if (rc < 0) {
    ed_log_error("unable to set gid to %d: %s", runas_gid, strerror(errno));
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
