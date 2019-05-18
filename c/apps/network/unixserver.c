#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "fnet.h"

#define BUFSIZE 1024

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
