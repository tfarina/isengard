#ifndef ED_NET_H_
#define ED_NET_H_

#include <stddef.h>

#define ED_NET_OK 0
#define ED_NET_ERR -1

int ed_net_tcp_socket_listen(char *host, int port, int backlog);
int ed_net_tcp_socket_accept(int sd, char *ipbuf, size_t ipbuf_len, short unsigned *port);

#endif /* ED_NET_H_ */
