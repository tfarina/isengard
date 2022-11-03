#ifndef FNET_H_
#define FNET_H_

#define FNET_OK 0
#define FNET_ERR -1

int fnet_tcp_socket_connect(char const *host, int port, int *out_fd);

#endif  /* FNET_H_ */
