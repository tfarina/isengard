#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

#include <pwd.h>

uid_t ed_privs_get_uid(char const *username);
gid_t ed_privs_get_gid(char const *username);

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
int ed_change_user(char const *username);

#endif  /* ED_PRIVS_H_ */
