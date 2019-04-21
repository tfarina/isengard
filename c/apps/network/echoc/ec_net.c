#include "ec_net.h"

#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ec_log.h"

int ec_net_tcp_socket_connect(char const *host, int port) {
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
    error("getaddrinfo failed: %s", gai_strerror(rv));
    return -1;
  }

  /* Loop through all the results and connect to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    sd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
    if (sd < 0) {
      error("socket failed: %s", strerror(errno));
      continue;
    }

    rv = connect(sd, cur->ai_addr, cur->ai_addrlen);
    if (rv < 0) {
      /* If we can't connect, try the next one. */
      close(sd);
      sd = -1;
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  /* Oops, we couldn't connect to any address. */
  if (sd == -1 && cur == NULL) {
    return -1;
  }

  return sd;
}
