#ifndef WRITEN_H_
#define WRITEN_H_

#include <stddef.h>

/* Write a line to a socket. */
void Writen(int fd, void *ptr, size_t nbytes);

#endif  // WRITEN_H_
