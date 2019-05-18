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

int main(int argc, char **argv)
{
        int sockfd;
        char neterr[FNET_ERR_LEN];
        const char *path;
        char buf[BUFSIZE];
        ssize_t n;
        char pre[10];
        int i;
        const char *space = " ";
        const char *newline = "\n";

        path = "server.socket";

	sockfd = fnet_unix_client(neterr, path);
	if (sockfd == FNET_ERR) {
	        return EXIT_FAILURE;
	}

        snprintf(pre, sizeof(pre), "NIXCT%d ", 1);

        if (write(sockfd, pre, strlen(pre)) <= 0) {
                fprintf(stderr, "could not write\n");
        }

        argc -= 1;
        argv += 1;

        for (i = 0; i < argc; i++) {
                printf(" %s", argv[i]);

                write(sockfd, space, strlen(space));

                write(sockfd, argv[i], strlen(argv[i]));
        }

        if (write(sockfd, newline, strlen(newline)) <= 0) {
                fprintf(stderr, "could not write\n");
        }

        while ((n = read(sockfd, buf, sizeof(buf))) > 0 ) {
                buf[n] = 0;
                fputs(buf, stdout);
        }

        return EXIT_SUCCESS;
}
