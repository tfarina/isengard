#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

void ed_privs_check_owner(char const *username);

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
int ed_privs_drop(char const *username);

#endif  /* ED_PRIVS_H_ */
