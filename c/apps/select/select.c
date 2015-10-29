/*
 * Author: beej.us
 * URL: http://beej.us/guide/bgnet/output/html/multipage/advanced.html#select
 * Slightly modified from http://beej.us/guide/bgnet/examples/select.c
 */

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

int main(void) {
  int fd = 0; /* File descriptor for standard input (STDIN). */
  double timeout = 2500.0; /* This is in milliseconds. */
  struct timeval tv;
  fd_set readfds;
  ssize_t rv;

  tv.tv_sec = (long)(timeout / 1000.0);
  tv.tv_usec = (long)(timeout * 1000.0) % 1000000;

  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);

  /* Don't care about writefds and exceptfds. */
  rv = select(fd + 1, &readfds, NULL, NULL, &tv);
  if (rv == 0) {
    printf("Timed out.\n");
    return -1;
  } else if (rv == -1) {
    /* TODO: Check errno for EAGAIN and EINTR. */
  }

  if (FD_ISSET(fd, &readfds))
    printf("A key was pressed.\n");
  else
    printf("Timed out.\n");

  return 0;
}
