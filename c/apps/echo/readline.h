#ifndef READLINE_H_
#define READLINE_H_

#include <sys/types.h>

ssize_t Readline(int fd, void *ptr, size_t maxlen);

#endif  // READLINE_H_
