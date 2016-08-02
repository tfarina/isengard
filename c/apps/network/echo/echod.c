/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BACKLOG 1024
#define BUFSIZE 8129

static unsigned int forked = 0; /* Number of child processes. */

static void logstatus(void) {
  printf("echod: status: %d\n", forked);
}

static void echo_stream(int fd) {
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

static void sigusr1_handler(int sig) {
  syslog(LOG_INFO, "shutdown by user");
  closelog();
  exit(EXIT_SUCCESS);
}

static const char *progname;

static void usage(void) {
  fprintf(stderr, "usage: %s [port]\n", progname);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int port;
  int tcpfd;
  int reuse = 1;
  int echofd;
  char strport[NI_MAXSERV], ntop[NI_MAXHOST];
  int ret;
  pid_t pid;

  progname = argv[0];

  if (argc != 2) {
    usage();
  }

  if (geteuid() != 0) {
    fprintf(stderr, "%s: need root privileges\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  openlog("echod", LOG_PID | LOG_NDELAY, LOG_DAEMON);

  port = atoi(argv[1]);
  snprintf(strport, sizeof(strport), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, "7", &hints, &addrlist)) != 0) {
    syslog(LOG_ERR, "getaddrinfo failed: %s", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((tcpfd = socket(cur->ai_family, cur->ai_socktype,
                        cur->ai_protocol)) == -1) {
      syslog(LOG_ERR, "socket failed: %s", strerror(errno));
      continue;
    }

    if (setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
      syslog(LOG_ERR, "set reuse addr on sd %d failed: %s", tcpfd, strerror(errno));
      close(tcpfd);
      continue;
    }

    if (bind(tcpfd, cur->ai_addr, cur->ai_addrlen) == -1) {
      syslog(LOG_ERR, "bind to port %s failed: %.200s", strport, strerror(errno));
      close(tcpfd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    syslog(LOG_ERR, "failed to bind");
    exit(EXIT_FAILURE);
  }

  if (listen(tcpfd, BACKLOG) == -1) {
    syslog(LOG_ERR, "listen on %d failed: %s", tcpfd, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((ret = getnameinfo(cur->ai_addr, cur->ai_addrlen,
                         ntop, sizeof(ntop), strport, sizeof(strport),
                         NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    syslog(LOG_ERR, "getnameinfo failed: %.100s", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  fprintf(stderr,
          "Server listening on %s port %d\n",
          ntop, port);

  signal(SIGCHLD, sigchld_handler);
  signal(SIGUSR1, sigusr1_handler);

  logstatus();

  while (1) {
    struct sockaddr_storage ss;
    struct sockaddr *sa = (struct sockaddr *)&ss;
    socklen_t sslen = sizeof(ss);

    if ((echofd = accept(tcpfd, sa, &sslen)) == -1) {
      syslog(LOG_ERR, "accept failed: %s", strerror(errno));
      continue;
    }

    if ((ret = getnameinfo(sa, sslen,
                           ntop, sizeof(ntop), strport, sizeof(strport),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      syslog(LOG_ERR, "getnameinfo failed: %.100s", gai_strerror(ret));
      exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "TCP connection from %s:%s", ntop, strport);
    printf("TCP connection from %s:%s\n", ntop, strport);

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
      echo_stream(echofd);
      break;

    default:
      close(echofd); /* we are the parent so look for another connection. */
      printf("echod: pid %d\n", pid);
    }
  }

  return EXIT_SUCCESS;
}
