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

static unsigned int forked = 0; /* Number of child processes. */

static void logstatus(void) {
  printf("echod: status: %d\n", forked);
}

static void handle_client(int fd) {
  char buf[BUFSIZE];
  int ret;

  while ((ret = read(fd, buf, sizeof(buf))) > 0 && write(fd, buf, ret) > 0);

  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);

  exit(EXIT_SUCCESS);
}

static void sigchld_handler(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    --forked;
    printf("echod: end %d status %d\n", pid, status);
    logstatus();
  }
  signal(SIGCHLD, sigchld_handler);
}

int main(int argc, char **argv) {
  struct sockaddr_in saddr;
  int port;
  int tcpfd;
  int reuse = 1;
  int echofd;
  char strport[NI_MAXSERV], ntop[NI_MAXHOST];
  int ret;
  pid_t pid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s #port-number\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[1]);
  snprintf(strport, sizeof(strport), "%d", port);

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(port);

  if ((ret = getnameinfo((struct sockaddr *)&saddr, sizeof(saddr),
                         ntop, sizeof(ntop), strport, sizeof(strport),
                         NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    die("getnameinfo failed: %.100s", gai_strerror(ret));
  }

  if ((tcpfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    die("socket failed: %s", strerror(errno));

  if (setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    die("set reuse addr on sd %d failed: %s", tcpfd, strerror(errno));

  if (bind(tcpfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    die("bind to port %s failed: %.200s", strport, strerror(errno));

  if (listen(tcpfd, BACKLOG) == -1) {
    close(tcpfd);
    die("listen on %d failed: %s", tcpfd, strerror(errno));
  }

  fprintf(stderr,
          "The server is now ready to accept connections on %s port %d\n",
          ntop, port);

  signal(SIGCHLD, sigchld_handler);

  logstatus();

  while (1) {
    struct sockaddr_storage ss;
    struct sockaddr *sa = (struct sockaddr *)&ss;
    socklen_t sslen = sizeof(ss);

    if ((echofd = accept(tcpfd, sa, &sslen)) == -1)
      die("accept failed");

    if ((ret = getnameinfo(sa, sslen,
                           ntop, sizeof(ntop), strport, sizeof(strport),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      die("getnameinfo failed: %.100s", gai_strerror(ret));
    }

    printf("Connection from %s:%s\n", ntop, strport);

    ++forked;
    logstatus();

    pid = fork();
    switch (pid) {
    case -1:
      close(echofd);
      --forked;
      logstatus();
      break;

    case 0:
      close(tcpfd);
      handle_client(echofd);
      break;

    default:
      close(echofd); /* we are the parent so look for another connection. */
      printf("echod: pid %d\n", pid);
    }
  }

  return EXIT_SUCCESS;
}
