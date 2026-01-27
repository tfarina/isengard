#ifndef _SECURITY_H_INCLUDED_
#define _SECURITY_H_INCLUDED_

/**
 * Drop root privileges.
 */
int drop_privileges(char const *username);

#endif  /* !defined(_SECURITY_H_INCLUDED_) */
