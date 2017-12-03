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

int main(void) {
        int sockfd;
        struct sockaddr_un sa;
        size_t salen;
        const char *path;
        int accept_fd;
        char buf[BUFSIZE];
        int rv;
        char magic[8];
        char cmdline[BUFSIZE];

        path = "server.socket";

        unlink(path);

        if ((sockfd = fnet_create_socket(AF_UNIX)) == FNET_ERR) {
                  return EXIT_FAILURE;
        }

        memset(&sa, 0, sizeof(sa));
        sa.sun_family = AF_UNIX;
        strncpy(sa.sun_path, path, sizeof(sa.sun_path));
        salen = strlen(path) + 1 + offsetof(struct sockaddr_un, sun_path);

        if (bind(sockfd, (const struct sockaddr*)&sa, salen) == -1) {
                fprintf(stderr, "bind() failed: %s\n", strerror(errno));
                close(sockfd);
                return EXIT_FAILURE;
        }

        if (listen(sockfd, SOMAXCONN) == -1) {
                fprintf(stderr, "listen() failed: %s\n", strerror(errno));
                close(sockfd);
                return EXIT_FAILURE;
        }

        sprintf(buf, "%s\r\n", "Target not found!");

	for (;;) {
                if ((accept_fd = accept(sockfd, NULL, NULL)) == -1) {
                        fprintf(stderr, "accept() failed: %s\n", strerror(errno));
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
