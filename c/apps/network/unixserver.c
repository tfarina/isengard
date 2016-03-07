#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int main(void) {
        struct sockaddr_un unix_addr;

        static const size_t kPathMax = sizeof(unix_addr.sun_path);

        memset(&unix_addr, 0, sizeof(unix_addr));
        unix_addr.sun_family = AF_UNIX;

        return 0;
}
