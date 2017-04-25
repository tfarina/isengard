#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char **argv) {
        int socket_fd;
        struct sockaddr_un unix_addr;
        const char *path;
        char buf[BUFSIZE];
        ssize_t n;
        char pre[10];
        int i;
        const char *space = " ";
        const char *newline = "\n";

        path = "server.socket";

        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
                fprintf(stderr, "failed to create AF_UNIX socket\n");
                return EXIT_FAILURE;
        }

        memset(&unix_addr, 0, sizeof(unix_addr));
        unix_addr.sun_family = AF_UNIX;
        strncpy(unix_addr.sun_path, path, sizeof(unix_addr.sun_path));

        if (connect(socket_fd, (struct sockaddr *)&unix_addr, sizeof(unix_addr)) == -1) {
        }

        snprintf(pre, sizeof(pre), "NIXCT%d ", 1);

        if (write(socket_fd, pre, strlen(pre)) <= 0) {
                fprintf(stderr, "could not write\n");
        }

        argc -= 1;
        argv += 1;

        for (i = 0; i < argc; i++) {
          printf(" %s", argv[i]);

          write(socket_fd, space, strlen(space));

          write(socket_fd, argv[i], strlen(argv[i]));
        }

        if (write(socket_fd, newline, strlen(newline)) <= 0) {
          fprintf(stderr, "could not write\n");
        }

        while ((n = read(socket_fd, buf, sizeof(buf))) > 0 ) {
                buf[n] = 0;
                fputs(buf, stdout);
        }

        return EXIT_SUCCESS;
}
