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

#include "ec_net.h"
#include "log.h"

#define BUFSIZE 4096

int main(int argc, char **argv) {
  int sockfd;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];

  if (argc != 3) {
    fprintf(stderr, "usage: %s [hostname] [port]\n", argv[0]);
    return -1;
  }

  sockfd = ec_net_tcp_socket_connect(argv[1], atoi(argv[2]));

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
