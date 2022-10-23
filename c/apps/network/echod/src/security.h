#ifndef _SECURITY_H_INCLUDED_
#define _SECURITY_H_INCLUDED_ 1

/**
 * Drop root privileges.
 */
int drop_privileges(char const *username);

#endif  /* !defined(_SECURITY_H_INCLUDED_) */
