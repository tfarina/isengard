#include "readline.h"

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

/* Max text line length. */
#define MAXLINE 4096

static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

static ssize_t my_read(int fd, char *ptr) {
  if (read_cnt <= 0) {
again:
    if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
      if (errno == EINTR) {
        goto again;
      }
      return -1;
    } else if (read_cnt == 0) {
      return 0;
    }
    read_ptr = read_buf;
  }
  read_cnt--;
  *ptr = *read_ptr++;
  return 1;
}

static ssize_t readline(int fd, void *vptr, size_t maxlen) {
  ssize_t n;
  ssize_t rc;
  char c;
  char *ptr;

  ptr =  (char*)vptr;
  for (n = 1; n < maxlen; n++) {
    if ((rc = my_read(fd, &c)) == 1) {
      *ptr++ = c;
      if (c == '\n') {
        /* newline is stored, like fgets() */
        break;
      }
    } else if (rc == 0) {
      *ptr = 0;
      return n - 1; /* EOF, n - 1 bytes were read */
    } else {
      return -1; /* error, errno set by read() */
    }
  }

  *ptr = 0; /* null terminate like fgets() */
  return n;
}

ssize_t Readline(int fd, void *ptr, size_t maxlen) {
  ssize_t n;

  if ((n = readline(fd, ptr, maxlen)) < 0) {
    //err_sys("readline error");
  }

  return n;
}
