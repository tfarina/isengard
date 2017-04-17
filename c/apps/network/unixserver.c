#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFSIZE 1024

int socket_read_line(int fd, char *buf, size_t max) {
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
        int socket_fd;
        struct sockaddr_un unix_addr;
        size_t unix_addr_len;
        int accept_fd;
        char buf[BUFSIZE];
        int rv;
        char magic[8];
        char cmdline[BUFSIZE];

        unlink("server.socket");

        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
                fprintf(stderr, "failed to create AF_UNIX socket\n");
                exit(EXIT_FAILURE);
        }

        memset(&unix_addr, 0, sizeof(unix_addr));
        unix_addr.sun_family = AF_UNIX;
        strncpy(unix_addr.sun_path, "server.socket", sizeof(unix_addr.sun_path));
        unix_addr_len = strlen("server.socket") + 1 + offsetof(struct sockaddr_un, sun_path);

        if (bind(socket_fd, (const struct sockaddr*)&unix_addr, unix_addr_len) == -1) {
                fprintf(stderr, "bind() failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }

        if (listen(socket_fd, SOMAXCONN) == -1) {
                fprintf(stderr, "listen() failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }

        sprintf(buf, "%s\r\n", "Target not found!");

	for (;;) {
                if ((accept_fd = accept(socket_fd, NULL, NULL)) == -1) {
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
