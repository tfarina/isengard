#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "die.h"

#define BUFLEN 512
#define PORT 5300

int main(void) {
  struct sockaddr_in servaddr, from;
  int sockfd;
  int recvlen;
  socklen_t fromlen;
  char buf[BUFLEN];

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    die("cannot create socket");

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    die("bind failed");

  for (;;) {
    recvlen = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
    if (recvlen > 0) {
      buf[recvlen] = '\0';
      printf("received message: %s\n", buf);
    }
  }

  return 0;
}
