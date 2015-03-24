#include "wrapsock.h"

#include <errno.h>
#include <sys/socket.h>

int Socket(int family, int type, int protocol) {
  int n;

  if ((n = socket(family, type, protocol)) < 0) {
    // err_sys("socket error");
  }

  return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
  if (bind(fd, sa, salen) < 0) {
    // err_sys("bind error");
  }
}

void Listen(int fd, int backlog) {
  if (listen(fd, backlog) < 0) {
    // err_sys("listen error");
  }
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
  int n;

again:
  if ((n = accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
    if (errno == EPROTO || errno == ECONNABORTED)
#else
    if (errno == ECONNABORTED)
#endif
      goto again;
    else {
      // err_sys("accept error");
    }
  }

  return 0;
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
  if (connect(fd, sa, salen) < 0) {
    //err_sys("connect error");
  }
}
