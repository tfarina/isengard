#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"

#define BUFLEN 512
#define ADDRESS NULL
#define PORT 5300

static int set_reuseaddr(int sd)
{
  int reuse = 1;
  return setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

int main(void) {
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  struct sockaddr_storage remoteaddr;
  socklen_t addrlen;
  int sockfd = 0;
  int recvlen;
  char buf[BUFLEN];
  int msgcnt = 0;  /* count # of messages we received */
  char strport[NI_MAXSERV];

  snprintf(strport, sizeof(strport), "%d", PORT);

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(ADDRESS, strport, &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((sockfd = socket(cur->ai_family, cur->ai_socktype,
                         cur->ai_protocol)) == -1) {
      error("cannot create socket");
      continue;
    }

    set_reuseaddr(sockfd);

    if (bind(sockfd, cur->ai_addr, cur->ai_addrlen) == -1) {
      error("bind on %d failed: %s", sockfd, strerror(errno));
      close(sockfd);
      continue;
  }

    break;
  }

  printf("listening on port %d\n", PORT);

  addrlen = sizeof(remoteaddr);
  for (;;) {
    recvlen = recvfrom(sockfd, buf, sizeof(buf), 0,
                       (struct sockaddr *)&remoteaddr, &addrlen);
    if (recvlen <= 0) {
     continue;
    }
    buf[recvlen] = '\0';
    printf("received message: %s\n", buf);

    sprintf(buf, "ack %d", msgcnt++);
    if (sendto(sockfd, buf, strlen(buf), 0,
               (struct sockaddr *)&remoteaddr, addrlen) == -1)
      fatal("sendto failed: %s", strerror(errno));
    printf("response sent: %s\n", buf);
  }

  return 0;
}
