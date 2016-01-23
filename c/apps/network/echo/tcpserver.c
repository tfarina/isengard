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
#include <sys/wait.h>
#include <unistd.h>

#include "die.h"

#define BACKLOG 1024
#define BUFSIZE 8129

static void handle_client(int fd) {
  char buf[BUFSIZE];
  int ret;

  while ((ret = read(fd, buf, sizeof(buf))) > 0 && write(fd, buf, ret) > 0);

  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);

  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  struct sockaddr_in servaddr;
  int port;
  int listen_fd;
  int reuse = 1;
  int client_fd;
  int pid;
  unsigned int num_children = 0; /* Number of child processes. */
  char strport[NI_MAXSERV], ntop[NI_MAXHOST];
  int ret;

  if (argc != 2) {
    fprintf(stderr, "usage: tcpserver #port-number\n");
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[1]);
  snprintf(strport, sizeof(strport), "%d", port);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if ((ret = getnameinfo((struct sockaddr *)&servaddr, sizeof(servaddr),
                         ntop, sizeof(ntop), strport, sizeof(strport),
                         NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    die("getnameinfo failed: %.100s", gai_strerror(ret));
  }

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    die("socket failed: %s", strerror(errno));

  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    die("set reuse addr on sd %d failed: %s", listen_fd, strerror(errno));

  if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
    die("bind to port %s failed: %.200s", strport, strerror(errno));

  if (listen(listen_fd, BACKLOG) == -1) {
    close(listen_fd);
    die("listen on %d failed: %s", listen_fd, strerror(errno));
  }

  fprintf(stderr,
          "The server is now ready to accept connections on %s port %d\n",
          ntop, port);

  while (1) {
    struct sockaddr_storage ss;
    struct sockaddr *sa = (struct sockaddr *)&ss;
    socklen_t addrlen = sizeof(ss);

    if ((client_fd = accept(listen_fd, sa, &addrlen)) == -1)
      die("accept failed");

    if ((ret = getnameinfo(sa, addrlen,
                           ntop, sizeof(ntop), strport, sizeof(strport),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      die("getnameinfo failed: %.100s", gai_strerror(ret));
    }

    printf("Connection from %s:%s\n", ntop, strport);

    ++num_children;
    printf("Num children: %d\n", num_children);

    switch (fork()) {
    case -1:
      close(client_fd);
      --num_children;
      printf("Num children: %d\n", num_children);
      break;
    case 0:
      close(listen_fd);
      handle_client(client_fd);
      break;
    default:
      close(client_fd);
    }
  }

  return EXIT_SUCCESS;
}
