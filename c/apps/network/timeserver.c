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
#define BUFSIZE 256

static unsigned int forked = 0;

static void logstatus(void) {
  printf("timeserver: status: %d\n", forked);
}

/* Echo the current day time to the connected client. */
static void doprocessing(int sockfd) {
  time_t t;
  char buf[BUFSIZE];

  time(&t);
  sprintf(buf, "%.24s\r\n", ctime(&t));
  write(sockfd, buf, strlen(buf));
  exit(EXIT_SUCCESS);
}

static void sigchld_handler(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    --forked;
    printf("timeserver: end %d status %d\n", pid, status);
    logstatus();
  }
  signal(SIGCHLD, sigchld_handler);
}

int main(void) {
  struct sockaddr_in servaddr;
  int sockfd;
  int client_fd;
  int reuse = 1;
  pid_t pid;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    die("cannot create socket");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    die("setsockopt failed");

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    die("bind failed");

  if (listen(sockfd, BACKLOG) == -1)
    die("listen failed");

  fprintf(stderr,
          "The server is now ready to accept connections on port %d\n",
          SERVER_PORT);

  signal(SIGCHLD, sigchld_handler);

  logstatus();

  while (1) {
    if ((client_fd = accept(sockfd, NULL, NULL)) == -1) {
      if (errno == EINTR) /* EINTR might happen on accept. */
        continue;
      die("accept failed");
    }

    ++forked;
    logstatus();

    pid = fork();
    switch (pid) {
      case -1:
        close(sockfd);
        close(client_fd);
        perror("fork failed");
        --forked;
        logstatus();
        break;

      case 0:
        close(sockfd);
        doprocessing(client_fd);

      default:
        close(client_fd); /* we are the parent so look for another connection. */
        printf("timeserver: pid %d\n", pid);
    }
  }

  return EXIT_SUCCESS;
}
