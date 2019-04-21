#ifndef EC_NET_H_
#define EC_NET_H_

int ec_net_tcp_socket_connect(char const *host, int port, int *out_sd);

#endif  /* EC_NET_H_ */
