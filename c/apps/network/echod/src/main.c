/*
 *  main.c - Echo Protocol main module
 *
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
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "daemon.h"
#include "ed_cmdline.h"
#include "ed_globals.h"
#include "ed_net.h"
#include "ed_privs.h"
#include "ed_version.h"
#include "options.h"
#include "os_path.h"
#include "pidfile.h"
#include "sig2str.h"
#include "ulog.h"
#include "unused-parameter.h"
#include "username.h"

#define BUFSIZE 8129

static int volatile signo = 0;
static sig_atomic_t volatile quit;
static sig_atomic_t volatile got_sigchld;
static int unsigned connected_clients = 0; /* Number of child processes. */

static void print_stats(void) {
  ulog_info("connected_clients=%d", connected_clients);
}

/**
 * Catch SIGCHLD signal.
 */
static void sigchld(int sig _GL_UNUSED_PARAMETER) {
  got_sigchld = 1;
}

static void reap_kids(void) {
  pid_t pid;
  int status;

  for (;;) {
    pid = waitpid(-1, &status, WNOHANG);
    if (pid <= 0) {
      break;
    }

    if (WIFEXITED(status)) {
      ulog_warn("pid %lu exited with status %d", (unsigned long) pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      ulog_warn("pid %lu killed with signal %d", (unsigned long) pid, WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
      ulog_warn("pid %lu stopped with signal %d", (unsigned long) pid, WSTOPSIG(status));
    } else {
      ulog_warn("pid %lu died for unknown reason", (unsigned long) pid);
    }

    --connected_clients;
    print_stats();
  }

  signal(SIGCHLD, sigchld);
}

/**
 * Catch SIGTERM signal and SIGINT signal (del/^C).
 */
static void sigterm(int sig) {
  quit = 1;
  signo = sig;
}

/**
 * Set up signal handlers.
 */
static void init_signals(void) {
  struct sigaction action;

  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  action.sa_handler = sigterm;

  if (sigaction(SIGINT, &action, (struct sigaction *) 0) < 0) {
    ulog_error("unable to set SIGINT: %s", strerror(errno));
  }
  if (sigaction(SIGTERM, &action, (struct sigaction *) 0) < 0) {
    ulog_error("unable to set SIGTERM: %s", strerror(errno));
  }

  action.sa_flags |= SA_NOCLDSTOP;
  action.sa_handler = sigchld;
  if (sigaction(SIGCHLD, &action, (struct sigaction *) 0) < 0) {
    ulog_error("unable to set SIGCHLD: %s", strerror(errno));
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

static int ed_event_loop(int fd) {
  char sigstr[SIG2STR_MAX];
  fd_set rfds_in;
  /* We need to have a copy of the fd set as it's not safe to reuse FD sets
   * after select(). */
  fd_set rfds_out;
  int maxfd = -1;
  int retval;
  char clientip[46];
  short unsigned clientport;
  int clientfd;
  pid_t pid;

  FD_ZERO(&rfds_in);

  FD_SET(fd, &rfds_in);

  if (maxfd < fd) {
    maxfd = fd;
  }

  for (;;) {
    if (quit == 1) {
      sig2str(signo, sigstr);
      ulog_info("terminating on signal %d (%s)", signo, sigstr);

      break;
    }

    memcpy(&rfds_out, &rfds_in, sizeof(fd_set));

    retval = select(maxfd + 1, &rfds_out, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

    if (retval > 0) {
      if (FD_ISSET(fd, &rfds_out)) {
	clientfd = ed_net_tcp_socket_accept(fd, clientip, sizeof(clientip), &clientport);
	if (clientfd == ED_NET_ERR) {
	  return -1;
	}

	ulog_info("connection from %s:%d", clientip, clientport);

	++connected_clients;
	print_stats();

	pid = fork();
	switch (pid) {
	/* Error */
	case -1:
	  close(clientfd);
	  --connected_clients;
	  print_stats();
	  break;

	/* Child process. */
	case 0:
	  ulog_info("child process forked");
	  close(fd);
	  echo_stream(clientfd);
	  break;

	/* Parent process. */
	default:
	  /*
	   * We are the parent so look for another connection.
	   */
	  close(clientfd);
	  ulog_info("new child created -- pid %d", pid);
	}
      }
    }

    if (got_sigchld) {
      got_sigchld = 0;
      reap_kids();
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  int rc;
  int tcpfd;

  /*
   * Set the umask to octal 022 to prevent clients from accidentally
   * creating group or world writable files.
   */
  umask(S_IWGRP | S_IWOTH);

  {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &act, (struct sigaction *) 0);
  }

  /*
   * Save our name for later usage.
   */
  progname_g = os_path_basename(*argv);

  /*
   * Initialize logging.
   */
  ulog_open(progname_g, (char const *) 0);

  /*
   * Check if it was run by the superuser.
   */
  if (getuid() != ROOT_UID) {
    ulog_fatal("must be run as root user");
  }

  /*
   * Initialize built-in parameters to their default values.
   */
  set_default_options(&opt);

  preparse_args(argc, argv, &opt);

  /*
   * Load configuration file.
   *
   * Configuration file settings override builtin defaults.
   *
   * If the user did not specify a config, read the system echod.conf.
   */

  /*
   * Process command-line.
   *
   * Command-line arguments override configuration file settings.
   */
  parse_args(argc, argv, &opt);

  ulog_open(progname_g, opt.logfile);

  if (opt.detach) {
    rc = daemonize();
    if (rc < 0) {
      ulog_error("Couldn't daemonize %s: %s\n", progname_g, strerror(errno));
      return rc;
    }
  }

  ed_privs_check_daemon_user(opt.user);

  ed_g_pid = getpid();

  /*
   * Write PID file after daemonizing.
   */
  if (opt.pidfile) {
    rc = pidfile_write(opt.pidfile, ed_g_pid);
    if (rc < 0) {
      return rc;
    }
  }

  tcpfd = ed_net_tcp_socket_listen(opt.address, opt.port, opt.backlog);
  if (tcpfd == ED_NET_ERR) {
    return EXIT_FAILURE;
  }

  /*
   * Drop root privileges.
   */
  rc = ed_privs_drop(ed_g_daemon_uid, ed_g_daemon_gid);
  if (rc < 0) {
    return rc;
  }

  init_signals();

  ulog_info("running as user '%s' (%ld) and group '%s' (%ld)",
            get_username(), (long)getuid(),
            get_groupname(), (long)getgid());

  ulog_info("starting daemon pid=%d version=%s", ed_g_pid, ED_VERSION_STR);

  ed_event_loop(tcpfd);

  pidfile_remove(opt.pidfile);
  ulog_close();

  return EXIT_SUCCESS;
}
