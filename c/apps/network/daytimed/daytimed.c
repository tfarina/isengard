/* Author: Paul Griffiths, 1999. paulgriffiths.net
 * URL: http://www.paulgriffiths.net/program/c/timeserv.php
 *
 * Code modified from paulgriffiths.net/program/c/srcs/timeservsrc.html
 * and http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
 *
 * $ ./daytimed &
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

#include "fnet.h"
#include "log.h"

#define DEFAULT_PORT 13
#define BACKLOG 1024
#define BUFSIZE 256
#define PROGNAME "daytimed"

static unsigned int forked = 0;

static void logstatus(void) {
  printf(PROGNAME ": status: %d\n", forked);
}

/* Echo the current day time to the connected client. */
static void daytime_stream(int sockfd) {
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
    printf(PROGNAME ": end %d status %d\n", pid, status);
    logstatus();
  }
  signal(SIGCHLD, sigchld_handler);
}

static void send_tcp_message(int sockfd) {
  int client_fd;
  pid_t pid;

  if ((client_fd = accept(sockfd, NULL, NULL)) == -1) {
    if (errno == EINTR) /* EINTR might happen on accept. */
      return;
    error("accept failed: %s", strerror(errno));
    exit(EXIT_FAILURE);
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
      daytime_stream(client_fd);

    default:
      close(client_fd); /* we are the parent so look for another connection. */
      printf(PROGNAME ": pid %d\n", pid);
  }
}

int main(void) {
  struct sockaddr_in servaddr;
  int sockfd;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(DEFAULT_PORT);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    error("cannot create socket: %s", strerror(errno));
    return EXIT_FAILURE;
  }

  if (fnet_set_reuseaddr(sockfd) == -1) {
    close(sockfd);
    return EXIT_FAILURE;
  }

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    error("bind on %d failed: %s", sockfd, strerror(errno));
    close(sockfd);
    return EXIT_FAILURE;
  }

  if (listen(sockfd, BACKLOG) == -1) {
    error("listen failed: %s", strerror(errno));
    close(sockfd);
    return EXIT_FAILURE;
  }

  info("The server is now ready to accept connections on port %d\n", DEFAULT_PORT);

  signal(SIGCHLD, sigchld_handler);

  logstatus();

  while (1) {
    send_tcp_message(sockfd);
  }

  return EXIT_SUCCESS;
}
