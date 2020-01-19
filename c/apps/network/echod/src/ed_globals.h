#ifndef ED_GLOBALS_H_
#define ED_GLOBALS_H_

#include <pwd.h>

/* The name the daemon were invoked with. */
extern char const *ed_g_progname;

extern int ed_g_pid; /* process: server id */

extern uid_t ed_g_daemon_uid;
extern gid_t ed_g_daemon_gid;

#endif  /* ED_GLOBALS_H_ */
