#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"

#define BUFLEN 512
#define ADDRESS NULL
#define PORT 5300

static int msgcnt;  /* count # of messages we received */

static int udp_socket_listen(char *host, int port) {
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int sockfd = 0;
  int reuse = 1;
  char strport[NI_MAXSERV];

  snprintf(strport, sizeof(strport), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(host, strport, &hints, &addrlist)) != 0) {
    error("getaddrinfo failed: %s", gai_strerror(rv));
    return -1;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((sockfd = socket(cur->ai_family, cur->ai_socktype,
                         cur->ai_protocol)) == -1) {
      error("cannot create socket: %s", strerror(errno));
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
      error("set reuse addr on sd %d failed: %s", sockfd, strerror(errno));
      close(sockfd);
      continue;
    }

    if (bind(sockfd, cur->ai_addr, cur->ai_addrlen) == -1) {
      error("bind on %d failed: %s", sockfd, strerror(errno));
      close(sockfd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    error("failed to bind");
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

static void send_udp_message(int sockfd) {
  int recvlen;
  char buf[BUFLEN];
  struct sockaddr_storage ss;
  socklen_t sslen = sizeof(ss);

  if ((recvlen = recvfrom(sockfd, buf, sizeof(buf), 0,
                          (struct sockaddr *)&ss, &sslen)) == -1) {
    error("recvfrom failed: %s", strerror(errno));
    return;
  }

  buf[recvlen] = '\0';

  printf("received message: %s\n", buf);

  sprintf(buf, "ack %d", msgcnt++);

  if (sendto(sockfd, buf, strlen(buf), 0,
             (struct sockaddr *)&ss, sslen) == -1) {
    error("sendto failed: %s", strerror(errno));
  }

  printf("response sent: %s\n", buf);
}

int main(int argc, char **argv) {
  int sockfd = 0;
  fd_set rfds_out;

  sockfd = udp_socket_listen(ADDRESS, PORT);

  printf("Server listening on port %d\n", PORT);

  FD_ZERO(&rfds_out);

  FD_SET(sockfd, &rfds_out);

  for (;;) {
    if (select(sockfd + 1, &rfds_out, NULL, NULL, NULL) < 0) {
      error("select failed: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }

    if (FD_ISSET(sockfd, &rfds_out)) {
      send_udp_message(sockfd);
    }
  }

  return 0;
}
