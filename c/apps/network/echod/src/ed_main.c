/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
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

#include "ed_config.h"
#include "ed_daemon.h"
#include "ed_instance.h"
#include "ed_log.h"
#include "ed_net.h"
#include "ed_pid.h"
#include "ed_rcode.h"
#include "ed_utils.h"

#define ED_VERSION_STRING "0.0.1"

#define BUFSIZE 8129

#define CRLF "\x0d\x0a"

static const char *progname;

static int show_help;
static int show_version;
static sig_atomic_t volatile quit;
static unsigned int connected_clients = 0; /* Number of child processes. */

static char short_options[] =
    "h"  /* help */
    "V"  /* version */
    "d"  /* daemon mode */
    "c:" /* configuration file */
    "o:" /* output logfile */
    "P:" /* pid file */
    "u:" /* user identity to run as */
    "l:" /* interface to listen on */
    "p:" /* tcp port number to listen on */
    "b:" /* tcp backlog queue limit */
    ;

static struct option long_options[] = {
    { "help",        no_argument,       NULL, 'h' }, /* help */
    { "version",     no_argument,       NULL, 'V' }, /* version */
    { "daemonize",   no_argument,       NULL, 'd' }, /* daemon mode */
    { "conf-file",   required_argument, NULL, 'c' },  /* configuration file */
    { "output-file", required_argument, NULL, 'o' }, /* output logfile */
    { "pid-file",    required_argument, NULL, 'P' }, /* pid file */
    { "user",        required_argument, NULL, 'u' }, /* user identity to run as */
    { "interface",   required_argument, NULL, 'l' }, /* interface to listen on */
    { "port",        required_argument, NULL, 'p' }, /* tcp port number to listen on */
    { "backlog",     required_argument, NULL, 'b' }, /* tcp backlog queue limit */
    { NULL,          0,                 NULL,  0  }
};

static void ed_show_usage(char const *program_name) {
  fprintf(stderr,
	  "usage: %s [-hd] [-o output logfile] [-P pid file] [-u user]" CRLF
	  "             [-l interface] [-p port] [-b backlog]" CRLF CRLF,
	  program_name);
  fprintf(stderr,
	  "options:" CRLF
          "  -h, --help              show usage, options and exit" CRLF
          "  -V, --version           show version and exit" CRLF
          "  -d, --daemonize         run as a daemon" CRLF
          "  -c, --conf-file=S       set configuration file" CRLF
          "  -o, --output-file=S     set the debug logging file (default: %s)" CRLF
          "  -P, --pid-file=S        store pid in a file (default: not stored)" CRLF
          "  -u, --user=S            user identity to run as" CRLF
          "  -l, --interface=S       interface to listen on (default: %s)" CRLF
          "  -p, --port=N            set the tcp port to listen on (default: %d)" CRLF
          "  -b, --backlog=N         the backlog argument of listen() applied to the" CRLF
          "                          listening socket (default: %d)" CRLF
	  "",
          "stderr",
          ED_INTERFACE != NULL ? ED_INTERFACE : "all interfaces",
          ED_TCP_PORT, ED_BACKLOG
	  );
}

static int ed_cmdline_parse(int argc, char **argv, ed_config_t *config) {
  int c, value;

  opterr = 0;

  for (;;) {
    c = getopt_long(argc, argv, short_options, long_options, NULL);
    if (c == -1) {
      /* no more options */
      break;
    }

    switch (c) {
    case 'h':
      show_help = 1;
      break;

    case 'V':
      show_version = 1;
      break;

    case 'd':
      config->daemonize = 1;
      break;

    case 'c':
      config->conf_filename = optarg;
      break;

    case 'o':
      config->log_filename = optarg;
      break;

    case 'P':
      config->pid_filename = optarg;
      break;

    case 'u':
      config->username = optarg;
      break;

    case 'l':
      config->interface = optarg;
      break;

    case 'p':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", progname);
	return ED_ERROR;
      }
      if (!ed_valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", progname, value);
	return ED_ERROR;
      }

      config->port = value;
      break;

    case 'b':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname);
	return ED_ERROR;
      }

      config->backlog = value;
      break;

    case '?':
      fprintf(stderr, "%s: invalid option -- '%c'\n", progname, optopt);
      return ED_ERROR;

    default:
      fprintf(stderr, "%s: invalid option -- '%c'\n", progname, optopt);
      return ED_ERROR;
      /* NOTREACHED */
    }
  }

  return ED_OK;
}

/**
 * This functions changes from the superuser (root) to the user
 * specified in 'username'. Effectively dropping the priviledges
 * that this application have.
 */
static int ed_change_user(struct passwd *pw, char const *username) {
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
  ed_log_info("connected_clients=%d", connected_clients);
}

static void sigchld_handler(int sig) {
  pid_t pid;
  int status;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    --connected_clients;
    ed_log_info("pid %lu with status %d", (unsigned long)pid, status);
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

  ed_log_info("signal %d (%s) received, shutting down...", sig, signame);
}

/**
 * Setup signals.
 */
static void ed_signal_init(void) {
  signal(SIGCHLD, sigchld_handler);
  signal(SIGINT, ed_signal_handler);
  signal(SIGTERM, ed_signal_handler);
}

static void echo_stream(int fd) {
  char buf[BUFSIZE];
  int ret;

  while ((ret = read(fd, buf, sizeof(buf))) > 0 && write(fd, buf, ret) > 0);

  sleep(1);  /* allow socket to drain before signalling the socket is closed */
  close(fd);

  exit(EXIT_SUCCESS);
}

static int ed_main_loop(int tcpfd) {
  fd_set rfds_in;
  /* We need to have a copy of the fd set as it's not safe to reuse FD sets
   * after select(). */
  fd_set rfds_out;
  int rc;
  char clientip[46];
  int clientport;
  int clientfd;
  pid_t pid;

  FD_ZERO(&rfds_in);

  FD_SET(tcpfd, &rfds_in);

  for (;;) {
    if (quit == 1) {
      break;
    }

    memcpy(&rfds_out, &rfds_in, sizeof(fd_set));

    rc = select(tcpfd + 1, &rfds_out, NULL, NULL, NULL);
    if (rc > 0) {
      if (FD_ISSET(tcpfd, &rfds_out)) {
        clientfd = ed_net_tcp_socket_accept(tcpfd, clientip, sizeof(clientip), &clientport);
	if (clientfd == ED_NET_ERR) {
	  return -1;
	}

	ed_log_info("Accepted connection from %s:%d", clientip, clientport);

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
          ed_log_info("pid: %d", pid);
        }
      }
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  ed_instance_t instance;
  ed_config_t config;
  struct passwd *pw;
  int rc;
  int tcpfd;

  progname = ed_get_progname(argv[0]);

  ed_instance_init(&instance);
  ed_config_init(&config);

  rc = ed_cmdline_parse(argc, argv, &config);
  if (rc != ED_OK) {
    ed_show_usage(progname);
    return EXIT_FAILURE;
  }

  if (show_help) {
    ed_show_usage(progname);
    return EXIT_SUCCESS;
  }

  if (show_version) {
    printf("%s version %s\n", progname, ED_VERSION_STRING);
    return EXIT_SUCCESS;
  }

  if (geteuid() != 0) {
    fprintf(stderr, "%s: need root privileges\n", progname);
    return EXIT_FAILURE;
  }

  pw = getpwnam(config.username);
  if (pw == NULL) {
    fprintf(stderr, "%s: cannot find user '%s' to switch to\n", progname, config.username);
    return EXIT_FAILURE;
  }

  rc = ed_log_init(config.log_filename);
  if (rc != ED_OK) {
    return rc;
  }

  if (config.daemonize) {
    rc = ed_daemon_detach(ED_MAXIMIZE_COREFILE);
    if (rc != ED_OK) {
      fprintf(stderr, "%s: unable to daemonize\n", progname);
      return rc;
    }
  }

  instance.pid = getpid();

  if (config.pid_filename != NULL) {
    rc = ed_pid_file_write(config.pid_filename, instance.pid);
    if (rc != ED_OK) {
      return rc;
    }
  }

  ed_signal_init();

  tcpfd = ed_net_tcp_socket_listen(config.interface, config.port, config.backlog);
  if (tcpfd == ED_NET_ERR) {
    return EXIT_FAILURE;
  }

  rc = ed_change_user(pw, config.username);
  if (rc != ED_OK) {
    return rc;
  }

  ed_log_info("%s started on %d, port %d, backlog %d, logfile %s, user %s", progname,
              instance.pid,
              config.port,
              config.backlog,
              config.log_filename,
              config.username);

  ed_main_loop(tcpfd);

  ed_log_info("Shutdown completed");
  ed_log_deinit();

  return EXIT_SUCCESS;
}
