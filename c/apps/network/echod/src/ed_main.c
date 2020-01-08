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

#include "ed_cmdline.h"
#include "ed_config.h"
#include "ed_daemon.h"
#include "ed_globals.h"
#include "ed_log.h"
#include "ed_net.h"
#include "ed_path.h"
#include "ed_pidfile.h"
#include "ed_privs.h"
#include "ed_rcode.h"
#include "ed_signals.h"
#include "ed_validate.h"

#define ED_ROOT_UID 0
#define BUFSIZE 8129

static sig_atomic_t volatile quit;
static sig_atomic_t volatile got_sigchld;
static int unsigned connected_clients = 0; /* Number of child processes. */

static void print_stats(void) {
  ed_log_info("connected_clients=%d", connected_clients);
}

static void sig_chld_handler(int sig) {
  got_sigchld = 1;
}

static void sig_chld_reaper(void) {
  pid_t pid;
  int status;

  for (;;) {
    pid = waitpid(-1, &status, WNOHANG);
    if (pid <= 0) {
      break;
    }

    ed_log_info("pid %lu died with exit status %d", (unsigned long)pid, WEXITSTATUS(status));
    --connected_clients;
    print_stats();
  }

  signal(SIGCHLD, sig_chld_handler);
}

static void sig_shutdown_handler(int sig) {
  if (sig == SIGTERM || sig == SIGINT) {
    quit = 1;
  }

  ed_log_info("signal %d (%s) received, shutting down...", sig, ed_sig2str(sig));
}

/**
 * Set up signal handlers.
 */
static void ed_sig_setup(void) {
  struct sigaction action;

  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  action.sa_handler = sig_shutdown_handler;

  if (sigaction(SIGINT, &action, (struct sigaction *) 0) < 0) {
    ed_log_error("unable to set SIGINT: %s", strerror(errno));
  }
  if (sigaction(SIGTERM, &action, (struct sigaction *) 0) < 0) {
    ed_log_error("unable to set SIGTERM: %s", strerror(errno));
  }

  action.sa_flags |= SA_NOCLDSTOP;
  action.sa_handler = sig_chld_handler;
  if (sigaction(SIGCHLD, &action, (struct sigaction *) 0) < 0) {
    ed_log_error("unable to set SIGCHLD: %s", strerror(errno));
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

static int ed_event_loop(int tcpfd) {
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
        /* Error */
        case -1:
          close(clientfd);
          --connected_clients;
          print_stats();
          break;

        /* Child process. */
        case 0:
          ed_log_info("child process forked");
          close(tcpfd);
          echo_stream(clientfd);
          break;

        /* Parent process. */
        default:
          /*
           * We are the parent so look for another connection.
           */
          close(clientfd);
          ed_log_info("pid: %d", pid);
        }
      }
    }

    if (got_sigchld) {
      got_sigchld = 0;
      sig_chld_reaper();
    }
  }

  return 0;
}

static char const *get_username(void) {
  uid_t uid;
  struct passwd *pw;

  uid = getuid();

  pw = getpwuid(uid);
  if (pw == 0) {
    return "unknown";
  }

  return pw->pw_name;
}

static char const *get_groupname(void) {
  gid_t gid;
  struct group *gr;

  gid = getgid();

  gr = getgrgid(gid);
  if (gr == 0) {
    return "unknown";
  }

  return gr->gr_name;
}

int main(int argc, char **argv) {
  ed_config_t config;
  int rc;
  int tcpfd;

  ed_g_progname = ed_path_basename(*argv);

  ed_log_set_ident(ed_g_progname);

  /*
   * Initialize built-in parameters to their default values.
   */
  ed_config_init(&config);

  /*
   * Check if it was run by the superuser.
   */
  if (geteuid() != ED_ROOT_UID) {
    ed_log_fatal("You must be root (uid = 0) to run %s", ed_g_progname);
  }

  /*
   * Process command-line.
   */
  rc = ed_cmdline_parse(argc, argv, &config);
  if (rc != ED_OK) {
    ed_cmdline_display_help();
    return EXIT_FAILURE;
  }

  if (show_help) {
    ed_cmdline_display_help();
    return EXIT_SUCCESS;
  }

  if (show_version) {
    ed_cmdline_display_version();
    return EXIT_SUCCESS;
  }

  /*
   * Load the configuration from the file.
   */
  ed_config_load_file(&config);

  if (config.logfile != NULL) {
    rc = ed_log_file_open(config.logfile);
    if (rc != ED_OK) {
      return rc;
    }
  }

  if (show_config) {
    fprintf(stdout, " *** %s configuration ***\n", ed_g_progname);
    fprintf(stdout, "username  = %s\n", config.username);
    fprintf(stdout, "conffile  = %s\n", config.conffile);
    fprintf(stdout, "pidfile   = %s\n", config.pidfile);
    fprintf(stdout, "logfile   = %s\n", config.logfile);
    fprintf(stdout, "port      = %d\n", config.port);
    fprintf(stdout, "backlog   = %d\n", config.backlog);
  }

  if (config.daemonize) {
    rc = ed_daemon_detach();
    if (rc != ED_OK) {
      ed_log_error("Couldn't daemonize %s: %s\n", ed_g_progname, strerror(errno));
      return rc;
    }
  }

  ed_privs_check_owner(config.username);

  ed_log_info("running as user '%s' (%ld) and group '%s' (%ld)",
	      get_username(), (long)getuid(),
              get_groupname(), (long)getgid());

  ed_g_pid = getpid();

  if (config.pidfile != NULL) {
    rc = ed_pidfile_write(config.pidfile, ed_g_pid);
    if (rc != ED_OK) {
      return rc;
    }
    /*if (chown(config.pidfile, ed_uid, ed_gid) == -1) {
      ed_log_error("unable to chown %u.%u %s: %s",
	           (unsigned)ed_uid, (unsigned)ed_gid,
                   config.pidfile, strerror(errno));
		   }*/
  }

  ed_sig_setup();

  tcpfd = ed_net_tcp_socket_listen(config.address, config.port, config.backlog);
  if (tcpfd == ED_NET_ERR) {
    return EXIT_FAILURE;
  }

  /*
   * Drop root privileges.
   */
  rc = ed_drop_privileges(config.username);
  if (rc != ED_OK) {
    return rc;
  }

  ed_log_info("running as user '%s' (%ld) and group '%s' (%ld)",
	      get_username(), (long)getuid(),
              get_groupname(), (long)getgid());

  ed_log_info("daemon started -- pid %d", ed_g_pid);

  ed_event_loop(tcpfd);

  ed_pidfile_remove(config.pidfile);
  ed_log_info("Shutdown completed");
  if (config.logfile != NULL) {
    ed_log_file_close();
  }

  return EXIT_SUCCESS;
}
