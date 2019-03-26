#include "ed_net.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ed_logger.h"

int ed_net_set_reuseaddr(int sd) {
  int reuse;

  reuse = 1;

  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    return ED_NET_ERR;
  }

  return ED_NET_OK;
}

int ed_net_tcp_socket_listen(char *host, int port, int backlog) {
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int tcpfd;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(host, portstr, &hints, &addrlist)) != 0) {
    ed_logger_log_error("getaddrinfo failed: %s", gai_strerror(rv));
    return ED_NET_ERR;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((tcpfd = socket(cur->ai_family, cur->ai_socktype,
                        cur->ai_protocol)) == -1) {
      ed_logger_log_error("socket failed: %s", strerror(errno));
      continue;
    }

    rv = ed_net_set_reuseaddr(tcpfd);
    if (rv != ED_NET_OK) {
      ed_logger_log_warn("set reuse addr on sd %d failed: %s", tcpfd, strerror(errno));
      close(tcpfd);
      continue;
    }

    if (bind(tcpfd, cur->ai_addr, cur->ai_addrlen) == -1) {
      ed_logger_log_error("bind to port %s failed: %.200s", portstr, strerror(errno));
      close(tcpfd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    ed_logger_log_error("failed to bind");
    return ED_NET_ERR;
  }

  if (listen(tcpfd, backlog) == -1) {
    ed_logger_log_error("listen on %d failed: %s", tcpfd, strerror(errno));
    close(tcpfd);
    return ED_NET_ERR;
  }

  return tcpfd;
}

static int ed_net_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen) {
  int fd;

  for (;;) {
    if ((fd = accept(sockfd, sa, salen)) == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        ed_logger_log_error("accept failed: %s", strerror(errno));
        return ED_NET_ERR;
      }
    }
    break;
  }

  return fd;
}

int ed_net_tcp_socket_accept(int tcpfd, char *ipbuf, size_t ipbuf_len, int *port) {
  struct sockaddr_storage ss;
  struct sockaddr *sa = (struct sockaddr *)&ss;
  socklen_t sslen = sizeof(ss);
  int fd;

  if ((fd = ed_net_generic_accept(tcpfd, sa, &sslen)) == ED_NET_ERR) {
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
