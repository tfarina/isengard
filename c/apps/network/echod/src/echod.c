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

#include "ed_instance.h"
#include "ed_log.h"
#include "ed_net.h"
#include "ed_utils.h"

#define ED_USER "_echod"

#define BUFSIZE 8129

#define CRLF "\x0d\x0a"

static const char *progname;

static int running = 0;
static unsigned int forked = 0; /* Number of child processes. */

static char short_options[] =
    "h"  /* help */
    "d"  /* daemon mode */
    "l:" /* interface to listen on */
    "p:" /* tcp port number to listen on */
    "b:" /* tcp backlog queue limit */
    ;

static struct option long_options[] = {
    { "help",      no_argument,       NULL, 'h' }, /* help */
    { "daemonize", no_argument,       NULL, 'd' }, /* daemon mode */
    { "interface", required_argument, NULL, 'l' }, /* interface to listen on */
    { "port",      required_argument, NULL, 'p' }, /* tcp port number to listen on */
    { "backlog",   required_argument, NULL, 'b' }, /* tcp backlog queue limit */
    { NULL,    0,                 NULL,  0  }
};

static void ed_show_usage(void) {
  fprintf(stderr,
	  "usage: %s [-hd] [-l interface] [-p port] [-b backlog]" CRLF CRLF,
	  progname);
  fprintf(stderr,
	  "options:" CRLF
          "  -h, --help         show usage, options and exit" CRLF
	  "  -d, --daemonize    run as a daemon" CRLF
          "  -l, --interface=S  interface to listen on (default: %s)" CRLF
          "  -p, --port=N       set the tcp port to listen on (default: %d)" CRLF
          "  -b, --backlog=N    the backlog argument of listen() applied to the" CRLF
	  "                     listening socket (default: %d)" CRLF
	  "",
          ED_INTERFACE != NULL ? ED_INTERFACE : "all interfaces",
          ED_TCP_PORT, ED_BACKLOG
	  );
}

static int drop_privileges(uid_t uid, gid_t gid) {
  if (setgroups(1, &gid) != 0) {
    fprintf(stderr, "setgroups failed\n");
    return -1;

  }
  if (setresgid(gid, gid, gid) != 0) {
    fprintf(stderr, "setresgid failed\n");
    return -1;
  }
  if (setresuid(uid, uid, uid) != 0) {
    fprintf(stderr, "setresuid failed\n");
    return -1;
  }

  return 0;
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

  if (sig == SIGTERM || sig == SIGINT) {
    running = 0;
  }

  log_info("signal %s received, shutting down...", type);
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
  int value;
  struct passwd *pw;
  int tcpfd;
  fd_set rfds_in;
  /* We need to have a copy of the fd set as it's not safe to reuse FD sets
   * after select(). */
  fd_set rfds_out;
  int clientfd;
  char clientip[46];
  int clientport;
  pid_t pid;
  ed_instance_t instance;

  ed_instance_init(&instance);

  progname = ed_get_progname(argv[0]);

  while ((ch = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
    switch (ch) {
    case 'd':
      instance.daemonize = 1;
      break;

    case 'l':
      instance.interface = optarg;
      break;

    case 'p':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", progname);
	return EXIT_FAILURE;
      }
      if (!ed_valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", progname, value);
	return EXIT_FAILURE;
      }

      instance.port = value;
      break;

    case 'b':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname);
	return EXIT_FAILURE;
      }

      instance.backlog = value;
      break;

    case 'h':
      ed_show_usage();
      return EXIT_SUCCESS;

    case '?':
      ed_show_usage();
      return EXIT_SUCCESS;

    default:
      ed_show_usage();
      exit(EXIT_FAILURE);
      /* NOTREACHED */
    }
  }
  argc -= optind;
  argv += optind;

  if (argc > 0) {
    ed_show_usage();
    return EXIT_FAILURE;
  }

  if (geteuid() != 0) {
    fprintf(stderr, "%s: need root privileges\n", progname);
    return EXIT_FAILURE;
  }

  if ((pw = getpwnam(ED_USER)) == NULL) {
    fprintf(stderr, "%s: cannot find user '%s' to switch to\n", progname, ED_USER);
    return EXIT_FAILURE;
  }

  if (instance.daemonize) {
    if (daemon(0, 0) == -1) {
      fprintf(stderr, "%s: unable to daemonize\n", progname);
      return EXIT_FAILURE;
    }
    /* update pid */
    instance.pid = getpid();
  }

  log_init(progname, !instance.daemonize);

  tcpfd = fnet_tcp_socket_listen(instance.interface, instance.port, instance.backlog);
  if (tcpfd == FNET_ERR) {
    return EXIT_FAILURE;
  }

  log_info("%s started on %d, port %d, backlog %d", progname, instance.pid, instance.port, instance.backlog);

  if (drop_privileges(pw->pw_uid, pw->pw_gid)) {
    return EXIT_FAILURE;
  }

  /* Setup signals. */
  signal(SIGCHLD, sigchld_handler);
  signal(SIGINT, handle_signal);
  signal(SIGTERM, handle_signal);

  print_num_child_forked();

  FD_ZERO(&rfds_in);

  FD_SET(tcpfd, &rfds_in);

  running = 1;

  while (running == 1) {
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

  closelog();
  log_info("Stopped");

  return EXIT_SUCCESS;
}
