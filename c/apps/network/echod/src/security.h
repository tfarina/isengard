#ifndef _SECURITY_H_INCLUDED_
#define _SECURITY_H_INCLUDED_ 1

#define ROOT_UID 0

void security_check_daemon_user(char const *name);

/**
 * Drop root privileges.
 */
int drop_privileges(char const *username);

#endif  /* !defined(_SECURITY_H_INCLUDED_) */
