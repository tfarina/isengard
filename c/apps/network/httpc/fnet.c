#include "fnet.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int fnet_tcp_socket_connect(const char *host, int port)
{
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int sd;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  printf("Resolving %s...\n", host);

  rv = getaddrinfo(host, portstr, &hints, &addrlist);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rv));
    return -1;
  }

  /* Loop through all the results and connect to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    char addr[NI_MAXHOST], strport[NI_MAXSERV];
    rv = getnameinfo(cur->ai_addr, cur->ai_addrlen,
                     addr, sizeof(addr), strport, sizeof(strport),
                     NI_NUMERICHOST | NI_NUMERICSERV);
    if (rv != 0) {
      fprintf(stderr, "getnameinfo failed: %.100s", gai_strerror(rv));
    }
    printf("Connecting to %.200s:%d...", addr, port);

    sd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
    if (sd < 0) {
      fprintf(stderr, "socket failed: %s\n", strerror(errno));
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

  if (sd == -1) {
    fprintf(stderr, "Failed to connect to %s\n", host);
    return -1;
  }

  printf("connected.\n");

  return sd;
}

