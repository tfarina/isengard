#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

void ed_privs_check_owner(char const *username);

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
int ed_drop_privileges(char const *username);

#endif  /* ED_PRIVS_H_ */
