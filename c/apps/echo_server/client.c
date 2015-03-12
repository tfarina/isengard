/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

#define SERVER_PORT 8088
#define MAXLINE 100

int main(int argc,char **argv) {
  int sockfd;
  char sendline[MAXLINE];
  char recvline[MAXLINE];
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);

  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

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
