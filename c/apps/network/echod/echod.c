/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ECHOD_USER "_echod"
#define ECHOD_INTERFACE NULL
#define ECHOD_TCP_PORT 7
#define ECHOD_BACKLOG 1024
#define BUFSIZE 8129

static const char *progname;

static unsigned int forked = 0; /* Number of child processes. */

static int log_on_stderr = 1;

static void log_init(int on_stderr) {
  log_on_stderr = on_stderr;

  if (!log_on_stderr)
    openlog(progname, LOG_PID | LOG_NDELAY, LOG_DAEMON);
}

static void vlog(int pri, const char *fmt, va_list ap) {
  char *nfmt;
  if (log_on_stderr) {
    /* Write to stderr */
    if (asprintf(&nfmt, "%s\n", fmt) == -1) {
      vfprintf(stderr, fmt, ap);
      fprintf(stderr, "\n");
    } else {
      vfprintf(stderr, nfmt, ap);
      free(nfmt);
    }
    fflush(stderr);
  } else {
    vsyslog(pri, fmt, ap);
  }
}

static void log_info(const char *emsg, ...) {
  va_list ap;

  va_start(ap, emsg);
  vlog(LOG_INFO, emsg, ap);
  va_end(ap);
}

static void log_error(const char *emsg, ...) {
  va_list ap;

  va_start(ap, emsg);
  vlog(LOG_ERR, emsg, ap);
  va_end(ap);
}

static void log_warning(const char *emsg, ...) {
  va_list ap;

  va_start(ap, emsg);
  vlog(LOG_WARNING, emsg, ap);
  va_end(ap);
}

#define FNET_OK 0
#define FNET_ERR -1

static int fnet_set_reuse_addr(int fd) {
  int reuse = 1;

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    log_error("setsockopt SO_REUSEADDR: %s", strerror(errno));
    return FNET_ERR;
  }

  return FNET_OK;
}

static int fnet_tcp_socket_listen(char *host, int port, int backlog) {
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int rv;
  int tcpfd;

  snprintf(portstr, sizeof(portstr), "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(host, portstr, &hints, &addrlist)) != 0) {
    log_error("getaddrinfo failed: %s", gai_strerror(rv));
    return FNET_ERR;
  }

  /* Loop through all the results and bind to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if ((tcpfd = socket(cur->ai_family, cur->ai_socktype,
                        cur->ai_protocol)) == -1) {
      log_error("socket failed: %s", strerror(errno));
      continue;
    }

    if (fnet_set_reuse_addr(tcpfd) == FNET_ERR) {
      close(tcpfd);
      continue;
    }

    if (bind(tcpfd, cur->ai_addr, cur->ai_addrlen) == -1) {
      log_error("bind to port %s failed: %.200s", portstr, strerror(errno));
      close(tcpfd);
      continue;
    }

    break;
  }

  freeaddrinfo(addrlist);

  if (cur == NULL) {
    log_error("failed to bind");
    return FNET_ERR;
  }

  if (listen(tcpfd, backlog) == -1) {
    log_error("listen on %d failed: %s", tcpfd, strerror(errno));
    close(tcpfd);
    return FNET_ERR;
  }

  return tcpfd;
}

static int fnet_generic_accept(int sockfd, struct sockaddr *sa, socklen_t *salen) {
  int fd;

  for (;;) {
    if ((fd = accept(sockfd, sa, salen)) == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        log_error("accept failed: %s", strerror(errno));
        return FNET_ERR;
      }
    }
    break;
  }

  return fd;
}

static int fnet_tcp_socket_accept(int tcpfd, char *ipbuf, size_t ipbuf_len, int *port) {
  struct sockaddr_storage ss;
  struct sockaddr *sa = (struct sockaddr *)&ss;
  socklen_t sslen = sizeof(ss);
  int fd;

  if ((fd = fnet_generic_accept(tcpfd, sa, &sslen)) == FNET_ERR) {
    return FNET_ERR;
  }

  if (ss.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&ss;
    if (ipbuf) {
      inet_ntop(AF_INET, (void*)&(s->sin_addr), ipbuf, ipbuf_len);
    }
    if (port) {
      *port = ntohs(s->sin_port);
    }
  } else {
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&ss;
    if (ipbuf) {
      inet_ntop(AF_INET6, (void*)&(s->sin6_addr), ipbuf, ipbuf_len);
    }
    if (port) {
      *port = ntohs(s->sin6_port);
    }
  }

  return fd;
}

static void print_num_child_forked(void) {
  log_info("num child forked: %d", forked);
}

static void sigchld_handler(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    --forked;
    log_info("pid %d status %d", pid, status);
    print_num_child_forked();
  }
  signal(SIGCHLD, sigchld_handler);
}

static void handle_signal(int sig) {
  char *type;

  switch (sig) {
  case SIGINT:
    type = "SIGINT";
    break;
  case SIGTERM:
    type = "SIGTERM";
    break;
  }

  log_info("signal %s received, shutting down...", type);
  closelog();
  exit(EXIT_SUCCESS);
}


static void usage(void) {
  fprintf(stderr, "usage: %s [-hd] [-p port]\n\n", progname);
  fprintf(stderr, "options:\n"
          " -h --help   show usage, options and exit\n"
	  " -d --debug  run in the foreground\n"
          " -p --port   set the tcp port to listen on (default: 7)\n");
}

static char *get_progname(char *argv0) {
  char *name;

  name = strrchr(argv0, '/');
  if (name == NULL) {
    name = argv0;
  } else {
    name++;
  }

  return name;
}

static int ec_valid_port(int port) {
  if (port < 1 || port > UINT16_MAX) {
    return 0;
  }

  return 1;
}

static void dropto(uid_t uid, gid_t gid) {
  if (setgroups(1, &gid) != 0) {
    fprintf(stderr, "setgroups failed\n");
    exit(EXIT_FAILURE);
  }
  if (setresgid(gid, gid, gid) != 0) {
    fprintf(stderr, "setresgid failed\n");
    exit(EXIT_FAILURE);
  }
  if (setresuid(uid, uid, uid) != 0) {
    fprintf(stderr, "setresuid failed\n");
    exit(EXIT_FAILURE);
  }
}

static void echo_stream(int fd) {
  char buf[BUFSIZE];
  int ret;

  while ((ret = read(fd, buf, sizeof(buf))) > 0 && write(fd, buf, ret) > 0);

  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);

  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  int ch;
  int debug = 0;
  int value;
  struct passwd *pw;
  char *host = ECHOD_INTERFACE;
  int port = ECHOD_TCP_PORT;
  int tcpfd;
  fd_set rfds_in;
  /* We need to have a copy of the fd set as it's not safe to reuse FD sets
   * after select(). */
  fd_set rfds_out;
  int stop = 0;
  int clientfd;
  char clientip[46];
  int clientport;
  pid_t pid;

  static struct option long_options[] = {
          {"help", no_argument, 0, 'h'},
          {"debug", no_argument, 0, 'd'},
          {"port", no_argument, 0, 'p'},
          {NULL, 0, 0, 0}
  };

  progname = get_progname(argv[0]);

  while ((ch = getopt_long(argc, argv, "hdp:", long_options, NULL)) != -1) {
    switch (ch) {
    case 'd':
      debug = 1;
      break;

    case 'p':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", progname);
	return EXIT_FAILURE;
      }
      if (!ec_valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid "
		   "port\n", progname, value);
	return EXIT_FAILURE;
      }

      port = value;
      break;

    case 'h':
      usage();
      return EXIT_SUCCESS;

    case '?':
      usage();
      return EXIT_SUCCESS;

    default:
      usage();
      exit(EXIT_FAILURE);
      /* NOTREACHED */
    }
  }
  argc -= optind;
  argv += optind;

  if (argc > 0) {
    usage();
    exit(EXIT_FAILURE);
  }

  if (geteuid() != 0) {
    fprintf(stderr, "%s: need root privileges\n", progname);
    exit(EXIT_FAILURE);
  }

  if ((pw = getpwnam(ECHOD_USER)) == NULL) {
    fprintf(stderr, "unknown user %s\n", ECHOD_USER);
    exit(EXIT_FAILURE);
  }

  if (!debug) {
    if (daemon(0, 0) == -1) {
      fprintf(stderr, "%s: unable to daemonize\n", progname);
      exit(EXIT_FAILURE);
    }
  }

  log_init(debug);

  tcpfd = fnet_tcp_socket_listen(host, port, ECHOD_BACKLOG);
  if (tcpfd == FNET_ERR) {
    return EXIT_FAILURE;
  }

  log_info("Server is ready to accept connections on port %d", port);

  dropto(pw->pw_uid, pw->pw_gid);

  signal(SIGCHLD, sigchld_handler);
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  print_num_child_forked();

  FD_ZERO(&rfds_in);

  FD_SET(tcpfd, &rfds_in);

  while (!stop) {
    memcpy(&rfds_out, &rfds_in, sizeof(fd_set));

    if (select(tcpfd + 1, &rfds_out, NULL, NULL, NULL) > 0) {
      if (FD_ISSET(tcpfd, &rfds_out)) {
        clientfd = fnet_tcp_socket_accept(tcpfd, clientip, sizeof(clientip), &clientport);
	if (clientfd == FNET_ERR) {
	  return EXIT_FAILURE;
	}

	log_info("Accepted connection from %s:%d", clientip, clientport);

        ++forked;
        print_num_child_forked();

        pid = fork();
        switch (pid) {
        case -1:
          close(clientfd);
          --forked;
          print_num_child_forked();
          break;

        case 0:
          close(tcpfd);
          echo_stream(clientfd);
          break;

        default:
          close(clientfd); /* we are the parent so look for another connection. */
         log_info("pid: %d", pid);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
