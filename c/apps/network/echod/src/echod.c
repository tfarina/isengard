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

#include "ed_daemon.h"
#include "ed_instance.h"
#include "ed_logger.h"
#include "ed_net.h"
#include "ed_pid.h"
#include "ed_rcode.h"
#include "ed_utils.h"

#define BUFSIZE 8129

#define CRLF "\x0d\x0a"

#define ED_MAXIMIZE_COREFILE 0

static const char *progname;

static sig_atomic_t quit;
static unsigned int connected_clients = 0; /* Number of child processes. */

static char short_options[] =
    "h"  /* help */
    "d"  /* daemon mode */
    "o:" /* output logfile */
    "P:" /* pid file */
    "u:" /* user identity to run as */
    "l:" /* interface to listen on */
    "p:" /* tcp port number to listen on */
    "b:" /* tcp backlog queue limit */
    ;

static struct option long_options[] = {
    { "help",      no_argument,       NULL, 'h' }, /* help */
    { "daemonize", no_argument,       NULL, 'd' }, /* daemon mode */
    { "output",    required_argument, NULL, 'o' }, /* output logfile */
    { "pid-file",  required_argument, NULL, 'P' }, /* pid file */
    { "user",      required_argument, NULL, 'u' }, /* user identity to run as */
    { "interface", required_argument, NULL, 'l' }, /* interface to listen on */
    { "port",      required_argument, NULL, 'p' }, /* tcp port number to listen on */
    { "backlog",   required_argument, NULL, 'b' }, /* tcp backlog queue limit */
    { NULL,    0,                 NULL,  0  }
};

static void ed_show_usage(void) {
  fprintf(stderr,
	  "usage: %s [-hd] [-o output logfile] [-P pid file] [-u user]" CRLF
	  "             [-l interface] [-p port] [-b backlog]" CRLF CRLF,
	  progname);
  fprintf(stderr,
	  "options:" CRLF
          "  -h, --help         show usage, options and exit" CRLF
	  "  -d, --daemonize    run as a daemon" CRLF
          "  -o, --output=S     set the debug logging file (default: %s)" CRLF
          "  -P, --pid-file=S   store pid in a file (default: not stored)" CRLF
          "  -u, --user=S       user identity to run as" CRLF
          "  -l, --interface=S  interface to listen on (default: %s)" CRLF
          "  -p, --port=N       set the tcp port to listen on (default: %d)" CRLF
          "  -b, --backlog=N    the backlog argument of listen() applied to the" CRLF
	  "                     listening socket (default: %d)" CRLF
	  "",
          "stderr",
          ED_INTERFACE != NULL ? ED_INTERFACE : "all interfaces",
          ED_TCP_PORT, ED_BACKLOG
	  );
}

static int drop_privileges(struct passwd *pw, char const *username) {
  gid_t gid;
  uid_t uid;

  gid = pw->pw_gid;
  uid = pw->pw_uid;

  if (setgroups(1, &gid) != 0) {
    fprintf(stderr, "setgroups failed\n");
    return ED_ERROR;
  }

  if (setresgid(gid, gid, gid) != 0) {
    fprintf(stderr, "%s: setting group id to user '%s' failed: %s\n",
            progname, username, strerror(errno));
    return ED_ERROR;
  }

  if (setresuid(uid, uid, uid) != 0) {
    fprintf(stderr, "%s: setting user id to user '%s' failed: %s\n",
            progname, username, strerror(errno));
    return ED_ERROR;
  }

  return ED_OK;
}

static void print_stats(void) {
  ed_logger_log_info("connected_clients=%d", connected_clients);
}

static void sigchld_handler(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    --connected_clients;
    ed_logger_log_info("pid %d status %d", pid, status);
    print_stats();
  }
  signal(SIGCHLD, sigchld_handler);
}

static void ed_signal_handler(int sig) {
  char *signame;

  switch (sig) {
  case SIGINT:
    signame = "SIGINT";
    break;
  case SIGTERM:
    signame = "SIGTERM";
    break;
  }

  if (sig == SIGTERM || sig == SIGINT) {
    quit = 1;
  }

  ed_logger_log_info("signal %d (%s) received, shutting down...", sig, signame);
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
  ed_instance_t instance;
  int c, value;
  struct passwd *pw;
  int rc;
  int tcpfd;
  fd_set rfds_in;
  /* We need to have a copy of the fd set as it's not safe to reuse FD sets
   * after select(). */
  fd_set rfds_out;
  char clientip[46];
  int clientport;
  int clientfd;
  pid_t pid;

  progname = ed_get_progname(argv[0]);

  ed_instance_init(&instance);

  for (;;) {
    c = getopt_long(argc, argv, short_options, long_options, NULL);
    if (c == -1) {
      /* no more options */
      break;
    }

    switch (c) {
    case 'h':
      ed_show_usage();
      return EXIT_SUCCESS;

    case 'd':
      instance.options.daemonize = 1;
      break;

    case 'o':
      instance.options.log_filename = optarg;
      break;

    case 'P':
      instance.options.pid_filename = optarg;
      break;

    case 'u':
      instance.options.username = optarg;
      break;

    case 'l':
      instance.options.interface = optarg;
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

      instance.options.port = value;
      break;

    case 'b':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname);
	return EXIT_FAILURE;
      }

      instance.options.backlog = value;
      break;

    case '?':
      ed_show_usage();
      return EXIT_SUCCESS;

    default:
      fprintf(stderr, "%s: invalid option -- '%c'", progname, optopt);
      return ED_ERROR;
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

  pw = getpwnam(instance.options.username);
  if (pw == NULL) {
    fprintf(stderr, "%s: cannot find user '%s' to switch to\n", progname, instance.options.username);
    return EXIT_FAILURE;
  }

  rc = ed_logger_init(instance.options.log_filename);
  if (rc != ED_OK) {
    return rc;
  }

  if (instance.options.daemonize) {
    rc = ed_daemonize(ED_MAXIMIZE_COREFILE);
    if (rc != ED_OK) {
      fprintf(stderr, "%s: unable to daemonize\n", progname);
      return rc;
    }
  }

  instance.pid = getpid();

  if (instance.options.pid_filename != NULL) {
    rc = ed_pid_create_file(instance.pid, instance.options.pid_filename);
    if (rc != ED_OK) {
      return rc;
    }
  }

  /* Setup signals. */
  signal(SIGCHLD, sigchld_handler);
  signal(SIGINT, ed_signal_handler);
  signal(SIGTERM, ed_signal_handler);

  tcpfd = ed_net_tcp_socket_listen(instance.options.interface, instance.options.port, instance.options.backlog);
  if (tcpfd == ED_NET_ERR) {
    return EXIT_FAILURE;
  }

  rc = drop_privileges(pw, instance.options.username);
  if (rc != ED_OK) {
    return rc;
  }

  ed_logger_log_info("%s started on %d, port %d, backlog %d, logfile %s, user %s", progname,
                     instance.pid,
                     instance.options.port,
                     instance.options.backlog,
                     instance.options.log_filename,
                     instance.options.username);

  FD_ZERO(&rfds_in);

  FD_SET(tcpfd, &rfds_in);

  for (;;) {
    if (quit == 1) {
      break;
    }

    memcpy(&rfds_out, &rfds_in, sizeof(fd_set));

    if (select(tcpfd + 1, &rfds_out, NULL, NULL, NULL) > 0) {
      if (FD_ISSET(tcpfd, &rfds_out)) {
        clientfd = ed_net_tcp_socket_accept(tcpfd, clientip, sizeof(clientip), &clientport);
	if (clientfd == ED_NET_ERR) {
	  return EXIT_FAILURE;
	}

	ed_logger_log_info("Accepted connection from %s:%d", clientip, clientport);

        ++connected_clients;
        print_stats();

        pid = fork();
        switch (pid) {
        case -1:
          close(clientfd);
          --connected_clients;
          print_stats();
          break;

        case 0:
          close(tcpfd);
          echo_stream(clientfd);
          break;

        default:
          close(clientfd); /* we are the parent so look for another connection. */
          ed_logger_log_info("pid: %d", pid);
        }
      }
    }
  }

  ed_logger_log_info("Terminated. Bye!");
  ed_logger_deinit();

  return EXIT_SUCCESS;
}
