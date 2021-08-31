#ifndef _FD_H_INCLUDED_
#define _FD_H_INCLUDED_ 1

#include <stddef.h>
#include <sys/types.h>

ssize_t fd_write(int fd, const char *buf, size_t len);
ssize_t fd_write_all(int fd, const char *buf, size_t len);
ssize_t fd_read(int fd, char *buf, size_t len);
ssize_t fd_read_all(int fd, char *buf, size_t len);

#endif  /* !defined(_FD_H_INCLUDED_) */
