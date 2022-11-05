/* Code modified from various sources
 * The overall structure from echo server:
 * https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
 * The HTTP headers from:
 * blog.manula.org/2011/05/writing-simple-web-server-in-c.html
 * */

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

#include "fnet.h"
#include "msg.h"

#define SERVER_PORT 8081
#define BACKLOG 1024
#define MAXLINE 4096

static const char hello_page[] = "<html><body><h1>Hello, world!</h1></body</html>";

static void handle_client(int fd) {
  char str[MAXLINE];
  char buffer[MAXLINE];

  read(fd, str, MAXLINE);

  sprintf(buffer,
          "HTTP/1.1 200 OK\nContent-Length: %u\nContent-Type: text/html\n\n%s",
          strlen(hello_page), hello_page);
  write(fd, buffer, strlen(buffer));

  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);
  exit(1);
}

int main(int argc, char *argv[]) {
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

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    fatal("cannot create socket");

  if (fnet_set_reuseaddr(listen_fd) != FNET_OK)
    fatal("fnet_set_reuseaddr failed");

  if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
    fatal("bind failed");

  if (listen(listen_fd, BACKLOG) == -1)
    fatal("listen failed");

  fprintf(stderr,
          "The server is now ready to accept connections on port %d\n",
          SERVER_PORT);

  while (1) {
    if ((client_fd = accept(listen_fd, (struct sockaddr*)&cliaddr, &clilen)) == -1)
      fatal("accept failed");

    printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

    if ((pid = fork()) < 0) {
      fatal("fork failed");
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
