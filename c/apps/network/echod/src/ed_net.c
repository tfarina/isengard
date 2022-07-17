#include "ed_net.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ulog.h"

#define ED_NET_OK 0
#define ED_NET_ERR -1

int ed_net_tcp_socket_listen(char *host, int port, int backlog) {
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int sockfd;
  int reuse = 1;

  rv = snprintf(portstr, sizeof(portstr), "%d", port);
  if (rv < 0 || (size_t) rv > sizeof(portstr)) {
    return ED_NET_ERR;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_NUMERICHOST;

  rv = getaddrinfo(host, portstr, &hints, &addrlist);
  if (rv != 0) {
    ulog_error("getaddrinfo failed: %s", gai_strerror(rv));
    return ED_NET_ERR;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    sockfd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
    if (sockfd < 0) {
      ulog_error("socket failed: %s", strerror(errno));
      continue;
    }

    rv = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (rv < 0) {
      ulog_error("setsockopt(SOL_SOCKET, SO_REUSEADDR) failed: %s", strerror(errno));
      close(sockfd);
      continue;
    }

    rv = bind(sockfd, cur->ai_addr, cur->ai_addrlen);
    if (rv < 0) {
      ulog_error("bind to %s:%s failed: %.200s", host, portstr, strerror(errno));
      close(sockfd);
      continue;
    }

    rv = listen(sockfd, backlog);
    if (rv < 0) {
      ulog_error("cannot listen on %s port %d: %s", host, port, strerror(errno));
      close(sockfd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    return ED_NET_ERR;
  }

  return sockfd;
}

static int ed_net_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen) {
  int fd;

  for (;;) {
    fd = accept(sockfd, sa, salen);
    if (fd < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        ulog_error("accept() failed: %s", strerror(errno));
        return ED_NET_ERR;
      }
    }
    break;
  }

  return fd;
}

int ed_net_tcp_socket_accept(int sockfd, char *ipbuf, size_t ipbuf_len, short unsigned *port) {
  struct sockaddr_storage ss;
  socklen_t sslen = sizeof(ss);
  int fd;

  fd = ed_net_generic_accept(sockfd, (struct sockaddr *) &ss, &sslen);
  if (fd == ED_NET_ERR) {
    return ED_NET_ERR;
  }

  if (ss.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&ss;
    if (ipbuf) {
      inet_ntop(AF_INET, (void*)&(s->sin_addr), ipbuf, ipbuf_len);
    }
    if (port) {
      *port = ntohs(s->sin_port);
    }
  } else {
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&ss;
    if (ipbuf) {
      inet_ntop(AF_INET6, (void*)&(s->sin6_addr), ipbuf, ipbuf_len);
    }
    if (port) {
      *port = ntohs(s->sin6_port);
    }
  }

  return fd;
}
