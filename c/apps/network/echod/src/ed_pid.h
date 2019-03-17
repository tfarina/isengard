#ifndef ED_PID_H_
#define ED_PID_H_

#include <sys/types.h>

void ed_pid_init(void);
int ed_pid_create(pid_t pid, char const *filename);
int ed_pid_unlink(char const *filename);

#endif  /* ED_PID_H_ */
