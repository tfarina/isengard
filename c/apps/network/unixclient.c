#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(void) {
        int socket_fd;
        struct sockaddr_un unix_addr;
        char buf[BUFSIZE];
        ssize_t n;
        char pre[10];

        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
                fprintf(stderr, "failed to create AF_UNIX socket\n");
                return EXIT_FAILURE;
        }

        memset(&unix_addr, 0, sizeof(unix_addr));
        unix_addr.sun_family = AF_UNIX;
        strncpy(unix_addr.sun_path, "server.socket", sizeof(unix_addr.sun_path));

        if (connect(socket_fd, (struct sockaddr *)&unix_addr, sizeof(unix_addr)) == -1) {
        }

        /* TODO: send command line to the server.*/
        snprintf(pre, sizeof(pre), "NIXCT%d ", 1);

        write(socket_fd, pre, strlen(pre));

        while ((n = read(socket_fd, buf, BUFSIZE)) > 0 ) {
                buf[n] = 0;
                fputs(buf, stdout);
        }

        return EXIT_SUCCESS;
}
