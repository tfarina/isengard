#include "ed_net.h"

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ed_log.h"

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
  int sd;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  rv = getaddrinfo(host, portstr, &hints, &addrlist);
  if (rv != 0) {
    ed_log_error("getaddrinfo failed: %s", gai_strerror(rv));
    return ED_NET_ERR;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    sd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
    if (sd < 0) {
      ed_log_error("socket failed: %s", strerror(errno));
      continue;
    }

    rv = ed_net_set_reuseaddr(sd);
    if (rv != ED_NET_OK) {
      ed_log_error("set reuse addr on sd %d failed: %s", sd, strerror(errno));
      close(sd);
      continue;
    }

    rv = bind(sd, cur->ai_addr, cur->ai_addrlen);
    if (rv < 0) {
      ed_log_error("bind to port %s failed: %.200s", portstr, strerror(errno));
      close(sd);
      continue;
    }

    rv = listen(sd, backlog);
    if (rv < 0) {
      ed_log_error("cannot listen on %s port %d: %s", host, port, strerror(errno));
      close(sd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    return ED_NET_ERR;
  }

  return sd;
}

static int ed_net_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen) {
  int fd;

  for (;;) {
    fd = accept(sockfd, sa, salen);
    if (fd == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        ed_log_error("accept() failed: %s", strerror(errno));
        return ED_NET_ERR;
      }
    }
    break;
  }

  return fd;
}

int ed_net_tcp_socket_accept(int sd, char *ipbuf, size_t ipbuf_len, int *port) {
  struct sockaddr_storage ss;
  struct sockaddr *sa = (struct sockaddr *)&ss;
  socklen_t sslen = sizeof(ss);
  int fd;

  if ((fd = ed_net_generic_accept(sd, sa, &sslen)) == ED_NET_ERR) {
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
