#ifndef ED_GLOBALS_H_
#define ED_GLOBALS_H_

#include <pwd.h>

/*
 * The name the program were called with.
 */
extern char const *progname_g;

extern int ed_g_pid;

extern uid_t ed_g_daemon_uid;
extern gid_t ed_g_daemon_gid;

#endif  /* ED_GLOBALS_H_ */
