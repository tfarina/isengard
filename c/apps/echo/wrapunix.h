#ifndef WRAPUNIX_H_
#define WRAPUNIX_H_

#include <sys/types.h>

pid_t Fork(void);
void Close(int fd);

#endif  // WRAPUNIX_H_
