/*
 * Author: beej.us
 * URL: http://beej.us/guide/bgnet/output/html/multipage/advanced.html#select
 * Slightly modified from http://beej.us/guide/bgnet/examples/select.c
 */

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

#define STDIN 0 /* file descriptor for standard input. */

int main(void) {
  double timeout = 2500.0; /* This is in milliseconds. */
  struct timeval tv;
  fd_set readfds;

  tv.tv_sec = (long)(timeout / 1000.0);
  tv.tv_usec = (long)(timeout * 1000.0) % 1000000;

  FD_ZERO(&readfds);
  FD_SET(STDIN, &readfds);

  /* Don't care about writefds and exceptfds. */
  select(STDIN + 1, &readfds, NULL, NULL, &tv);

  if (FD_ISSET(STDIN, &readfds))
    printf("A key was pressed.\n");
  else
    printf("Timed out.\n");

  return 0;
}
