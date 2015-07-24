/* Code modified from various sources
 * The overall structure from echo server:
 * https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
 * The HTTP headers from:
 * blog.manula.org/2011/05/writing-simple-web-server-in-c.html
 * */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "die.h"

#define SERVER_PORT 8081
#define LISTENQ 1024
#define MAXLINE 4096

static void handle_client(int fd) {
  char str[MAXLINE];
  read(fd, str, MAXLINE);
  write(fd, "HTTP/1.1 200 OK\n", 16);
  write(fd, "Content-Length: 49\n", 19);
  write(fd, "Content-Type: text/html\n\n", 25);
  write(fd, "<html><body><h1>Hello, World.</h1></body></html>", 49);
  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);
  exit(1);
}

int main() {
  struct sockaddr_in servaddr;
  int listen_fd;
  struct sockaddr_in cliaddr;
  socklen_t clilen = sizeof(cliaddr);
  int client_fd;
  int pid;

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0)
    die("cannot create socket");

  if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    die("bind failed");

  if (listen(listen_fd, LISTENQ) < 0)
    die("listen failed");

  fprintf(stderr,
          "The server is now ready to accept connections on port %d\n",
          SERVER_PORT);

  while (1) {
    if ((client_fd = accept(listen_fd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
      die("accept failed");

    printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

    if ((pid = fork()) < 0) {
      die("fork failed");
    } else {
      if (pid == 0) {	/* child */
	(void)close(listen_fd);
        handle_client(client_fd);
      } else {	/* parent */
        (void)close(client_fd);
      }
    }
  }

  return EXIT_SUCCESS;
}
