#ifndef ED_NET_H_
#define ED_NET_H_

#include <stddef.h>

#define FNET_OK 0
#define FNET_ERR -1

int fnet_set_reuseaddr(int fd);
int fnet_tcp_socket_listen(char *host, int port, int backlog);
int fnet_tcp_socket_accept(int tcpfd, char *ipbuf, size_t ipbuf_len, int *port);

#endif /* ED_NET_H_ */
