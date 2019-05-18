#include "fnet.h"

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
  int sockfd;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  printf("Resolving %s...\n", host);

  if ((rv = getaddrinfo(host, portstr, &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rv));
    return -1;
  }

  /* Loop through all the results and connect to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    char addr[NI_MAXHOST], strport[NI_MAXSERV];
    if ((rv = getnameinfo(cur->ai_addr, cur->ai_addrlen,
                          addr, sizeof(addr), strport, sizeof(strport),
                          NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      fprintf(stderr, "getnameinfo failed: %.100s", gai_strerror(rv));
    }
    printf("Connecting to %.200s:%d...", addr, port);

    if ((sockfd = socket(cur->ai_family, cur->ai_socktype,
                         cur->ai_protocol)) == -1) {
      break;
    }

    if (connect(sockfd, cur->ai_addr, cur->ai_addrlen) == 0) {
      printf("connected.\n");
      break;
    }

    close(sockfd);
    sockfd = -1;
  }

  freeaddrinfo(addrlist);

  if (sockfd == -1) {
    fprintf(stderr, "Failed to connect to %s\n", host);
    return -1;
  }

  return sockfd;
}

