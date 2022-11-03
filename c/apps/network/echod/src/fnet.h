#ifndef FNET_H_
#define FNET_H_

#include <stddef.h>

#define FNET_OK 0
#define FNET_ERR -1

int fnet_tcp_socket_listen(char *host, int port, int backlog);
int fnet_tcp_socket_accept(int sockfd, char *ipbuf, size_t ipbuf_len, short unsigned *port);

#endif /* FNET_H_ */
