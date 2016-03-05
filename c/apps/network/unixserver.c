#include <stddef.h>
#include <sys/un.h>

int main(void) {
        struct sockaddr_un unix_addr;

        static const size_t kPathMax = sizeof(unix_addr.sun_path);

        return 0;
}
