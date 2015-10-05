#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "die.h"

#define BUFLEN 512
#define PORT 5300

int main(int argc, char **argv) {
  struct sockaddr_in servaddr;
  socklen_t addrlen = sizeof(servaddr);
  int sockfd;
  char buf[BUFLEN] = "hello udp server";
  int recvlen;

  if (argc != 2) {
    fprintf(stderr, "usage: udpclient #server-ip-address\n");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    die("cannot create socket");

  printf("sending message to %s:%d\n", argv[1], PORT);

  if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, addrlen) == -1)
    die("sendto failed: %s", strerror(errno));
  printf("message sent: %s\n", buf);

  recvlen = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, &addrlen);
  if (recvlen > 0) {
    buf[recvlen] = '\0';
    printf("received response: %s\n", buf);
  }

  return 0;
}
