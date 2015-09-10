#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(PORT);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    die("cannot create socket");

  printf("sending message: \"%s\" to %s on port %d\n", buf, argv[1], PORT);
  sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, addrlen);

  recvlen = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, &addrlen);
  if (recvlen > 0) {
    buf[recvlen] = '\0';
    printf("received response: %s\n", buf);
  }

  return 0;
}
