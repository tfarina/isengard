#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "log.h"

#define BUFLEN 512
#define PORT 5300

int main(int argc, char **argv) {
  struct addrinfo hints, *addrlist;
  int rv;
  struct sockaddr_storage from;
  socklen_t fromlen = sizeof(from);
  int sockfd;
  char buf[BUFLEN] = "hello udp server";
  int recvlen;

  if (argc != 2) {
    fprintf(stderr, "usage: %s [hostname]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(argv[1], "5300", &hints, &addrlist)) != 0) {
    error("getaddrinfo failed: %s", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  if (addrlist == NULL) {
    error("no addrlist");
    exit(EXIT_FAILURE);
  }

  if ((sockfd = socket(addrlist->ai_family, addrlist->ai_socktype, 0)) == -1) {
    error("cannot create socket: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("sending message to %s:%d\n", argv[1], PORT);

  if (sendto(sockfd, buf, strlen(buf), 0,
             addrlist->ai_addr, addrlist->ai_addrlen) == -1) {
    error("sendto failed: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("message sent: %s\n", buf);

  recvlen = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from,
                     &fromlen);

  if (recvlen <= 0) {
    warning("can't receive reply");
    exit(EXIT_FAILURE);
  }

  buf[recvlen] = '\0';
  printf("received response: %s\n", buf);

  return 0;
}
