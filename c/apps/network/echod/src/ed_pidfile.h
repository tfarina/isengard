#ifndef ED_PIDFILE_H_
#define ED_PIDFILE_H_

#include <sys/types.h>

int ed_pidfile_write(char const *filename, pid_t pid);
int ed_pidfile_unlink(char const *filename);

#endif  /* ED_PIDFILE_H_ */
