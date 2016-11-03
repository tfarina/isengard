/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"

#define BUFSIZE 4096

int tcp_socket_connect(char *host, int port) {
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
    error("getaddrinfo failed: %s", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  /* Loop through all the results and connect to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((tcpfd = socket(cur->ai_family, cur->ai_socktype,
                        cur->ai_protocol)) == -1) {
      error("socket failed: %s", strerror(errno));
      break;
    }

    if (connect(tcpfd, cur->ai_addr, cur->ai_addrlen) == 0) {
      break;
    }

    /* If we can't connect, try the next one. */
    close(tcpfd);
    tcpfd = -1;
  }

  /* Oops, we couldn't connect to any address. */
  if (tcpfd == -1 && cur == NULL) {
    freeaddrinfo(addrlist);
    return -1;
  }

  freeaddrinfo(addrlist);
  return tcpfd;
}

int main(int argc, char **argv) {
  int sockfd;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];

  if (argc != 3) {
    fprintf(stderr, "usage: %s [hostname] [port]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  sockfd = tcp_socket_connect(argv[1], atoi(argv[2]));

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  while (1) {
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);

    read(sockfd, recvline, BUFSIZE);
    fputs(recvline, stdout);
  }

  return 0;
}
