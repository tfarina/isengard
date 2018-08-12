/* Code from unpv13e's tcpcliserv/tcpcli01.c */

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wraplib.h"
#include "wrapsock.h"
#include "str_cli.h"

#define SERV_PORT 8088

static void usage(void)
{
  fprintf(stderr, "usage: tcpcli01 <IP address>\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2) {
    usage();
  }

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  str_cli(stdin, sockfd);

  return 0;
}
