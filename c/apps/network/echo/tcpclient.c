/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "die.h"

#define BUFSIZE 4096

int main(int argc, char **argv) {
  struct sockaddr_in servaddr;
  int port;
  int sockfd;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];

  if (argc != 3) {
    fprintf(stderr, "usage: tcpclient #server-ip-address #port-number\n");
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[2]);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  servaddr.sin_port = htons(port);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    die("socket failed: %s", strerror(errno));

  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  while (1) {
    bzero(sendline, BUFSIZE);
    bzero(recvline, BUFSIZE);
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);
    read(sockfd, recvline, BUFSIZE);
    printf("%s", recvline);
  }

  return 0;
}
