/* Code from unpv13e's tcpcliserv/tcpserv01.c */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "wrapsock.h"
#include "wrapunix.h"
#include "str_echo.h"

#define SERV_PORT 8088
#define LISTENQ 1024

int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  fprintf(stderr, "Server is listening on port %d\n", SERV_PORT);

  for (;;) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

    if ((childpid = Fork()) == 0) {
      Close(listenfd);

      str_echo(connfd);

      exit(0);
    }
    Close(connfd);
  }
}
