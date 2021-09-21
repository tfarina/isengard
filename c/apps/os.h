#ifndef _OS_H_INCLUDED_
#define _OS_H_INCLUDED_ 1

/**
 * Returns the local machine's host name as reported by the kernel.
 */
char *os_hostname(void);

/**
 * Returns the current user's home directory.
 */
char *os_homedir(void);

/**
 * Returns the user's configuration directory, where information like
 * user preferences and settings can be stored.
 */
char *os_userconfigdir(void);

/**
 * Returns the user name of the current user.
 */
char *os_username(void);

#endif  /* !defined(_OS_H_INCLUDED_) */
