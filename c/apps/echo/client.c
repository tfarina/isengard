/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 8088
#define MAXLINE 4096

int main(int argc, char **argv) {
  struct sockaddr_in servaddr;
  int sockfd;
  char sendline[MAXLINE];
  char recvline[MAXLINE];

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
  servaddr.sin_port = htons(SERVER_PORT);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  while (1) {
    bzero(sendline, MAXLINE);
    bzero(recvline, MAXLINE);
    fgets(sendline, MAXLINE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);
    read(sockfd, recvline, MAXLINE);
    printf("%s", recvline);
  }

  return 0;
}
