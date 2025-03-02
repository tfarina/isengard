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

/* config.h must be included first. */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

#include "cfgfile.h"
#include "daemonize.h"
#include "echod.h"
#include "fnet.h"
#include "id.h"
#include "options.h"
#include "os_path.h"
#include "pathnames.h"
#include "pidfile.h"
#include "security.h"
#include "sig2str.h"
#include "ulog.h"
#include "unused-parameter.h"

#define BUFSIZE 8129

char const *progname;

static int volatile signo = 0;
static sig_atomic_t volatile gotsigterm;
static sig_atomic_t volatile gotsigchld;
static int unsigned connected_clients = 0; /* Number of child processes. */

static void print_stats(void) {
  ulog_info("connected_clients=%d", connected_clients);
}

/**
 * Catch SIGCHLD signal.
 */
static void sigchld(int sig _GL_UNUSED_PARAMETER) {
  gotsigchld = 1;
}

static void reap_child(void) {
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
      ulog_warn("pid %lu terminated by signal %d", (unsigned long) pid, WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
      ulog_warn("pid %lu stopped by signal %d", (unsigned long) pid, WSTOPSIG(status));
    } else {
      ulog_warn("pid %lu died for unknown reason", (unsigned long) pid);
    }

    --connected_clients;
    print_stats();
  }

  signal(SIGCHLD, sigchld);
}

/**
 * Catch SIGTERM signal and SIGINT signal (del/^C) so that we can exit
 * gracefully.
 */
static void sigterm(int sig) {
  gotsigterm = 1;
  signo = sig;
}

/**
 * Set up the signal handlers.
 */
static void init_signals(void) {
  struct sigaction action;

  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  action.sa_handler = sigterm;

  if (sigaction(SIGINT, &action, (struct sigaction *) 0) < 0) {
    ulog_error("Failed to set up the signal handler for SIGINT: %s", strerror(errno));
  }
  if (sigaction(SIGTERM, &action, (struct sigaction *) 0) < 0) {
    ulog_error("Failed to set up the signal handler for SIGTERM: %s", strerror(errno));
  }

  action.sa_flags |= SA_NOCLDSTOP;
  action.sa_handler = sigchld;
  if (sigaction(SIGCHLD, &action, (struct sigaction *) 0) < 0) {
    ulog_error("Failed to set up the signal handler for SIGCHLD: %s", strerror(errno));
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

static int main_loop(int fd) {
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
    if (gotsigterm == 1) {
      sig2str(signo, sigstr);
      ulog_info("received signal %d %s", signo, sigstr);

      break;
    }

    memcpy(&rfds_out, &rfds_in, sizeof(fd_set));

    retval = select(maxfd + 1, &rfds_out, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

    if (retval > 0) {
      if (FD_ISSET(fd, &rfds_out)) {
	clientfd = fnet_tcp_socket_accept(fd, clientip, sizeof(clientip), &clientport);
	if (clientfd < 0) {
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

    if (gotsigchld) {
      gotsigchld = 0;
      reap_child();
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  int rc;
  int pid;
  int tcpfd;
  struct passwd *pw;

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
   * Save the program name for logging, etc.
   */
  progname = os_path_basename(*argv);

  /*
   * Initialize logging.
   */
  ulog_open(progname);

  /*
   * Make sure the program was run by root (aka SuperUser).
   */
  if (!id_is_root()) {
    ulog_fatal("you should run this program as super-user (root).");
  }

  /*
   * Initialize built-in parameters to their default values.
   */
  init_options();

  /*
   * This is separate to find the configuration file if the user provided one
   * and read it before the other user options.
   */
  preparse_args(argc, argv);

  /*
   * Load configuration file.
   *
   * Configuration file settings override built-in defaults.
   *
   * If the user did not specify a config, read the system echod.conf.
   */
  read_config_file(opt.conffile);

  /*
   * Process command-line.
   *
   * Command-line arguments override configuration file settings.
   */
  parse_args(argc, argv);

  if (opt.dump_options) {
    dump_options();
    return 0;
  }

  /*
   * Now that configuration file and command line has been parsed, honor either
   * the config setting or the option '--logfile'.
   */
  ulog_set_file(opt.logfile);

  /*
   * Bind to tcp socket as soon as possible.
   */
  tcpfd = fnet_tcp_socket_listen(opt.address, opt.port, opt.backlog);
  if (tcpfd < 0) {
    return 1;
  }

  pw = getpwnam(opt.user);
  if (pw == 0) {
    ulog_fatal("user '%s' not found", opt.user);
  }

  rc = mkdir(DEF_PIDDIR, 0750);
  if (rc < 0 && errno != EEXIST) {
    ulog_warn("Could not create " DEF_PIDDIR ": %s", strerror(errno));
  } else {
    rc = chown(DEF_PIDDIR, pw->pw_uid, pw->pw_gid);
    if (rc < 0) {
      ulog_error("chown() failed: %s", strerror(errno));
    }
  }

  if (opt.detach) {
    rc = daemonize();
    if (rc < 0) {
      ulog_error("unable to daemonize: %s", strerror(errno));
      return 1;
    }
  }

  pid = getpid();

  if (opt.pidfile) {
    /*
     * Creating the PID file in /var/run directory needs root privilege.
     */
    rc = pidfile_create(opt.pidfile);
    if (rc < 0) {
      return 1;
    }

    /*
     * Only write the PID file after daemonizing.
     */
    rc = pidfile_write(opt.pidfile, pid);
    if (rc < 0) {
      return 1;
    }

    /*
     * Change the PID file owner, to be able to remove it later.
     */
    rc = chown(opt.pidfile, pw->pw_uid, pw->pw_gid);
    if (rc < 0) {
      ulog_error("chown() failed: %s", strerror(errno));
    }
  }

  /*
   * Release root privileges and run as this user.
   *
   * This should be done only after getpwnam, writing pidfile, etc.
   */
  rc = drop_privileges(opt.user);
  if (rc < 0) {
    return 1;
  }

  ulog_notice("running as user '%s' (%ld) and group '%s' (%ld)",
              id_get_username(), (long)getuid(),
              id_get_groupname(), (long)getgid());

  init_signals();

  ulog_notice("echo daemon %s started -- pid=%d", PACKAGE_VERSION, pid);

  main_loop(tcpfd);

  pidfile_remove(opt.pidfile);
  ulog_close();

  return 0;
}
