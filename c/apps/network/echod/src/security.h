#ifndef SECURITY_H
#define SECURITY_H

/**
 * Drop root privileges.
 */
int drop_privileges(char const *username);

#endif /* SECURITY_H */
