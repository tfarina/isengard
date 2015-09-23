/* Author: Paul Griffiths, 1999. paulgriffiths.net
 * URL: http://www.paulgriffiths.net/program/c/timeserv.php
 *
 * Code modified from paulgriffiths.net/program/c/srcs/timeservsrc.html
 * and http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
 *
 * $ ./timeserver &
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include "die.h"

#define SERVER_PORT 8088
#define BACKLOG 1024
#define MAXLINE 4096

/* Echo the current day time to the connected client. */
static void doprocessing(int sockfd) {
  time_t t;
  char str[MAXLINE];

  time(&t);
  sprintf(str, "%.24s\r\n", ctime(&t));
  write(sockfd, str, strlen(str));
  exit(EXIT_SUCCESS);
}

static void reapchld(int sig) {
  while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
  struct sockaddr_in servaddr;
  int listen_fd;
  int client_fd;
  int opt = 1;

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

  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = reapchld;
  sigemptyset(&act.sa_mask);
  sigaction(SIGCHLD, &act, 0);

  while (1) {
    if ((client_fd = accept(listen_fd, NULL, NULL)) == -1) {
      if (errno == EINTR) /* EINTR might happen on accept. */
        continue;
      die("accept failed");
    }

    switch (fork()) {
      case -1:
        close(listen_fd);
        close(client_fd);
        die("fork failed");

      case 0:
        close(listen_fd);
        doprocessing(client_fd);

      default:
        close(client_fd); /* we are the parent so look for another connection. */
        continue;
    }
  }

  return EXIT_SUCCESS;
}
