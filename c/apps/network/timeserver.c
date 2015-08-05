/* Author: Paul Griffiths, 1999. paulgriffiths.net
 * URL: http://www.paulgriffiths.net/program/c/timeserv.php
 *
 * Code modified from paulgriffiths.net/program/c/srcs/timeservsrc.html
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

int main() {
  struct sockaddr_in servaddr;
  int listen_fd;
  int client_fd;
  time_t current_time;
  char str[MAXLINE];
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

  while (1) {
    if ((client_fd = accept(listen_fd, NULL, NULL)) == -1)
      die("accept failed");

    /* Echo the time to the connected client. */
    current_time = time(NULL);
    sprintf(str, "%s\n", ctime(&current_time));
    Writen(client_fd, str, strlen(str));

    if (close(client_fd) < 0)
      die("close failed");
  }

  return EXIT_SUCCESS;
}
