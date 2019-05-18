#ifndef FNET_H_
#define FNET_H_

#include <sys/socket.h>

#define FNET_ERR -1

int fnet_create_socket(int domain);

int fnet_unix_server(const char *path, int backlog);

int fnet_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen);

int fnet_unix_socket_accept(int sockfd);

#endif  /* FNET_H_ */
