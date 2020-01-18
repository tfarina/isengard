#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

#include <pwd.h>

void ed_privs_check_owner(char const *username);

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
int ed_privs_drop(char const *username, uid_t uid, gid_t gid);

#endif  /* ED_PRIVS_H_ */
