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
    fprintf(stderr, "usage: %s #server-ip-address #port-number\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[2]);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    die("socket failed: %s", strerror(errno));

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    die("connect failed: %s", strerror(errno));

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  while (1) {
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);

    read(sockfd, recvline, BUFSIZE);
    fputs(recvline, stdout);
  }

  return 0;
}
