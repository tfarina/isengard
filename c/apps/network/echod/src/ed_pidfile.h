#ifndef ED_PIDFILE_H_
#define ED_PIDFILE_H_

#include <sys/types.h>

int ed_pidfile_write(char const *pidfile_path, pid_t pid);
int ed_pidfile_remove(char const *pidfile_path);

#endif  /* ED_PIDFILE_H_ */
