/*
 * Author: beej.us
 * URL: http://beej.us/guide/bgnet/output/html/multipage/advanced.html#select
 * Slightly modified from http://beej.us/guide/bgnet/examples/select.c
 */

#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

static int wait(double timeout) {
  int fd = 0; /* File descriptor for standard input (STDIN). */
  int maxfd = -1;
  fd_set readfds;
  struct timeval tv;
  ssize_t nfds;

  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);

  if (maxfd < fd) {
    maxfd = fd;
  }

  tv.tv_sec = (long)(timeout / 1000.0);
  tv.tv_usec = (long)(timeout * 1000.0) % 1000000;

  printf("active file descriptors (max %d): ", maxfd);
  for (nfds = 0; nfds <= maxfd; ++nfds) {
    if (FD_ISSET(nfds, &readfds)) {
      printf("%d ", nfds);
    }
  }
  putc('\n', stdout);

  /*
   * Watch out read events.
   */
  nfds = select(maxfd + 1, &readfds, NULL, NULL, &tv);
  if (nfds == -1) {
    /* TODO: Check errno for EAGAIN. */
    if (errno == EINTR) { /* ^C was pressed. */
      return -1;
    }
  } else if (nfds == 0) {
    printf("Timed out.\n");
    return -1;
  }

  printf("\nSuccessful select, descriptor count = %d\n", nfds);

  if (FD_ISSET(fd, &readfds)) {
    printf("A key was pressed.\n");
  } else {
    printf("Timed out.\n");
  }

  return 0;
}

int main(void) {
  double timeout = 2500.0; /* This is in milliseconds. */

  wait(timeout);

  return 0;
}
