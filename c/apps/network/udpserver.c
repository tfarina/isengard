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

#define FNET_OK 0
#define FNET_ERR -1
#define FNET_ERR_LEN 256

static void fnet_set_error(char *err, const char *fmt, ...) {
  va_list ap;

  if (!err) {
    return;
  }

  va_start(ap, fmt);
  vsnprintf(err, FNET_ERR_LEN, fmt, ap);
  va_end(ap);
}


static int fnet_set_reuseaddr(int fd, char *err) {
  int reuse = 1;

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    fnet_set_error(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
    return FNET_ERR;
  }

  return FNET_OK;
}

static int fnet_udp_socket_listen(char *host, int port) {
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int sockfd = 0;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_flags = AI_PASSIVE;

  rv = getaddrinfo(host, portstr, &hints, &addrlist);
  if (rv != 0) {
    error("getaddrinfo failed: %s", gai_strerror(rv));
    return FNET_ERR;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((sockfd = socket(cur->ai_family, cur->ai_socktype,
                         cur->ai_protocol)) == -1) {
      error("cannot create socket: %s", strerror(errno));
      continue;
    }

    if (fnet_set_reuseaddr(sockfd, NULL) == FNET_ERR) {
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
    return -1;
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
