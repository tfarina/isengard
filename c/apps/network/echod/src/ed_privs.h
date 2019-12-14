#ifndef ED_PRIVS_H_
#define ED_PRIVS_H_

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
int ed_change_user(char const *username);

#endif  /* ED_PRIVS_H_ */
