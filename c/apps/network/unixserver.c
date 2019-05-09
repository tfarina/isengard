#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFSIZE 1024

#define FNET_ERR -1

static int fnet_create_socket(int domain)
{
  int sockfd;

  if ((sockfd = socket(domain, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "failed to create socket\n");
    return FNET_ERR;
  }

  return sockfd;
}

static int fnet_unix_server(const char *path, int backlog)
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

static int socket_read_line(int fd, char *buf, size_t max) {
  int rv;
  size_t len = 0;

  while (len < max) {
    if ((rv = read(fd, buf + len, 1)) <= 0) {
      return 0;
    }

    if (buf[len] == '\n') {
      /* Return string without \n. */
      buf[len] = 0;
      return 1;
    }
    len++;
  }

  buf[max - 1] = 0;
  return 0;
}

static int fnet_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen) {
  int fd;

  for (;;) {
    fd = accept(sockfd, sa, salen);
    if (fd == -1) {
      if (errno == EINTR) {
	continue;
      } else {
        fprintf(stderr, "accept() failed: %s\n", strerror(errno));
        return -1;
      }
    }
    break;
  }

  return fd;
}

static int fnet_unix_socket_accept(int sockfd) {
  int fd;
  struct sockaddr_un sa;
  socklen_t salen = sizeof(sa);

  if ((fd = fnet_generic_accept(sockfd, (struct sockaddr *)&sa, &salen)) == -1) {
    return -1;
  }

  return fd;
}

int main(void) {
        int sockfd;
        const char *path;
        int accept_fd;
        char buf[BUFSIZE];
        int rv;
        char magic[8];
        char cmdline[BUFSIZE];

        path = "server.socket";

        unlink(path);

	sockfd = fnet_unix_server(path, SOMAXCONN);
	if (sockfd == FNET_ERR) {
	        return EXIT_FAILURE;
	}

        sprintf(buf, "%s\r\n", "Target not found!");

	for (;;) {
                if ((accept_fd = fnet_unix_socket_accept(sockfd)) == -1) {
                        exit(EXIT_FAILURE);
                }

                if ((rv = read(accept_fd, magic, (int)sizeof(magic) - 1)) <= 0) {
                  fprintf(stderr, "could not read\n");
                }

                magic[7] = 0;

                if (rv != 7 || strncmp(magic, "NIXCT", 5) != 0) {
                  fprintf(stderr, "control connection has bad header\n");
                }

                printf("%s\n", magic);

                if (!socket_read_line(accept_fd, cmdline, sizeof(cmdline))) {

                }
                printf("%s\n", cmdline);

                write(accept_fd, buf, strlen(buf));

                if (close(accept_fd) < 0) {

                }
        }

        return EXIT_SUCCESS;
}
