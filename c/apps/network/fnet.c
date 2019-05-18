#include "fnet.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int fnet_create_socket(int domain)
{
  int sockfd;

  if ((sockfd = socket(domain, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "failed to create socket\n");
    return FNET_ERR;
  }

  return sockfd;
}

int fnet_unix_server(const char *path, int backlog)
{
  int sockfd;
  struct sockaddr_un sa;
  size_t salen;

  if ((sockfd = fnet_create_socket(AF_UNIX)) == FNET_ERR) {
    return FNET_ERR;
  }

  memset(&sa, 0, sizeof(sa));
  sa.sun_family = AF_UNIX;
  strncpy(sa.sun_path, path, sizeof(sa.sun_path));
  salen = strlen(path) + 1 + offsetof(struct sockaddr_un, sun_path);

  if (bind(sockfd, (const struct sockaddr*)&sa, salen) == -1) {
    fprintf(stderr, "bind() failed: %s\n", strerror(errno));
    close(sockfd);
    return FNET_ERR;
  }

  if (listen(sockfd, backlog) == -1) {
    fprintf(stderr, "listen() failed: %s\n", strerror(errno));
    close(sockfd);
    return FNET_ERR;
  }

  return sockfd;
}

int fnet_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen)
{
  int fd;

  for (;;) {
    fd = accept(sockfd, sa, salen);
    if (fd == -1) {
      if (errno == EINTR) {
	continue;
      } else {
        fprintf(stderr, "accept() failed: %s\n", strerror(errno));
        return FNET_ERR;
      }
    }
    break;
  }

  return fd;
}

int fnet_unix_socket_accept(int sockfd)
{
  int fd;
  struct sockaddr_un sa;
  socklen_t salen = sizeof(sa);

  if ((fd = fnet_generic_accept(sockfd, (struct sockaddr *)&sa, &salen)) == -1) {
    return -1;
  }

  return fd;
}
