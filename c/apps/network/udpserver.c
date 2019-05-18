#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fnet.h"
#include "log.h"

#define BUFLEN 512
#define ADDRESS NULL
#define PORT 5300

static int msgcnt;  /* count # of messages we received */

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

  sockfd = fnet_udp_socket_listen(ADDRESS, PORT);

  printf("Server listening on port %d\n", PORT);

  FD_ZERO(&rfds_out);

  FD_SET(sockfd, &rfds_out);

  for (;;) {
    if (select(sockfd + 1, &rfds_out, NULL, NULL, NULL) > 0) {
      if (FD_ISSET(sockfd, &rfds_out)) {
        send_udp_message(sockfd);
      }
    }
  }

  return 0;
}
