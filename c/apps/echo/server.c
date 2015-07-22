/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

#include "die.h"

#define SERVER_PORT 8088
#define LISTENQ 1024
#define MAXLINE 100

int main() {
  struct sockaddr_in servaddr;
  int listen_fd;
  int conn_fd;
  char str[MAXLINE];

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0)
    die("cannot create socket");

  if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    die("bind failed");

  if (listen(listen_fd, LISTENQ) < 0)
    die("listen failed");

  conn_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

  while (1) {
    bzero(str, MAXLINE);

    read(conn_fd, str, MAXLINE);

    printf("Echoing back - %s", str);

    write(conn_fd, str, strlen(str) + 1);
  }

  return 0;
}
