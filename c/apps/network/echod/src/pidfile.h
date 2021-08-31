#ifndef _PIDFILE_H_INCLUDED_
#define _PIDFILE_H_INCLUDED_ 1

/**
 * Functions for creating and removing PID files.
 */

#include <sys/types.h>

int pidfile_create(char const *pidfile_path);
int pidfile_write(char const *pidfile_path, pid_t pid);
int pidfile_remove(char const *pidfile_path);

#endif  /* !defined(_PIDFILE_H_INCLUDED_) */
