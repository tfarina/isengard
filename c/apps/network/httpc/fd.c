#include "fd.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

ssize_t fd_write(int fd, const char *buf, size_t len)
{
  ssize_t num_written; /* On success, the number of bytes written (sent). */

  do {
    num_written = write(fd, buf, len);
  } while (num_written == -1 && errno == EINTR);

  return num_written;
}

ssize_t fd_write_all(int fd, const char *buf, size_t len)
{
  ssize_t total_written = 0; /* Total number of bytes which were written (sent). */

  while (len > 0) {
    ssize_t num_written; /* On success, the number of bytes which were written (sent). */

    num_written = fd_write(fd, buf, len);

    if (num_written < 0)
      return num_written;

    if (num_written == 0)
      break;

    printf("[DEBUG]: number of bytes sent: %zd\n", num_written);

    total_written += num_written;

    buf += num_written;
    len -= num_written;
  }

  return total_written;
}

ssize_t fd_read(int fd, char *buf, size_t len)
{
  ssize_t rv;

  do {
    rv = read(fd, buf, len);
  } while (rv == -1 && errno == EINTR);

  return rv;
}

ssize_t fd_read_all(int fd, char *buf, size_t len)
{
  char *b = buf;

  while (len) {
    ssize_t bytes_received;

    bytes_received = fd_read(fd, b, len);
    if (bytes_received < 0)
      return -1;
    if (bytes_received == 0)
      break;

    b += bytes_received;
    len -= bytes_received;
  }

  return b - buf;
}
