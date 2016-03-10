#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(void) {
        int socket_fd;
        struct sockaddr_un unix_addr;

        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
                fprintf(stderr, "failed to create AF_UNIX socket\n");
                exit(EXIT_FAILURE);
        }

        static const size_t kPathMax = sizeof(unix_addr.sun_path);

        memset(&unix_addr, 0, sizeof(unix_addr));
        unix_addr.sun_family = AF_UNIX;

        return 0;
}
