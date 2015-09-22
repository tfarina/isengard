/* Author: Paul Griffiths, 1999. paulgriffiths.net
 * URL: http://www.paulgriffiths.net/program/c/timeserv.php
 *
 * Code modified from paulgriffiths.net/program/c/srcs/timeservsrc.html
 * and http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
 *
 * $ ./timeserver &
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "die.h"
#include "util.h"

#define SERVER_PORT 8088
#define BACKLOG 1024
#define MAXLINE 4096

/* Echo the current day time to the connected client. */
static void process_request(int sockfd) {
  time_t t;
  char str[MAXLINE];

  time(&t);
  sprintf(str, "%.24s\r\n", ctime(&t));
  Writen(sockfd, str, strlen(str));
}

int main(int argc, char **argv) {
  struct sockaddr_in servaddr;
  int listen_fd;
  int client_fd;
  int opt = 1;
  pid_t childpid;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    die("cannot create socket");

  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    die("setsockopt failed");

  if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    die("bind failed");

  if (listen(listen_fd, BACKLOG) == -1)
    die("listen failed");

  fprintf(stderr,
          "The server is now ready to accept connections on port %d\n",
          SERVER_PORT);

  while (1) {
    if ((client_fd = accept(listen_fd, NULL, NULL)) == -1)
      die("accept failed");

    /* Create child process. */
    if ((childpid = fork()) == -1)
      die("fork failed");

    if (childpid == 0) {
      /* Client process. */
      close(listen_fd);
      process_request(client_fd);
      exit(EXIT_SUCCESS);
    }

    /* Parent process. */
    close(client_fd);
  }

  return EXIT_SUCCESS;
}
