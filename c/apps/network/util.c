#include "util.h"

#include <errno.h>
#include <unistd.h>

/* Code from "Unix Network Programming" - unpv13e's lib/writen.c */

static ssize_t writen(int fd, const void *vptr, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;

  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
      if (nwritten < 0 && errno == EINTR) {
        nwritten = 0;
      } else {
        return -1;
      }
    }
    nleft -= nwritten;
    ptr += nwritten;
  }

  return n;
}

void Writen(int fd, void *ptr, size_t nbytes) {
  if (writen(fd, ptr, nbytes) != nbytes) {
    //err_sys("Writen error");
  }
}
