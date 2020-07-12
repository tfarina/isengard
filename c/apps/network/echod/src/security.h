#ifndef _SECURITY_H_INCLUDED_
#define _SECURITY_H_INCLUDED_ 1

#include <pwd.h>

#define ROOT_UID 0

extern uid_t ed_g_daemon_uid;
extern gid_t ed_g_daemon_gid;

void security_check_daemon_user(char const *user);

/**
 * Drop root privileges.
 */
int drop_privileges(uid_t uid, gid_t gid);

#endif  /* !defined(_SECURITY_H_INCLUDED_) */
