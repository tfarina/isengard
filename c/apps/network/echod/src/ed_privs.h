#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

#include <pwd.h>

#define ROOT_UID 0

void ed_privs_check_daemon_user(char const *user);

/**
 * Drop root privileges.
 */
int ed_privs_drop(uid_t uid, gid_t gid);

#endif  /* ED_PRIVS_H_ */
