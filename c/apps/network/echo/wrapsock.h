#ifndef WRAPSOCK_H_
#define WRAPSOCK_H_

#include <sys/socket.h>

int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);

#endif  // WRAPSOCK_H_
