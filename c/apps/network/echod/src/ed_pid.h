#ifndef ED_PID_H_
#define ED_PID_H_

#include <sys/types.h>

int ed_pid_file_write(char const *filename, pid_t pid);
int ed_pid_file_unlink(char const *filename);

#endif  /* ED_PID_H_ */
