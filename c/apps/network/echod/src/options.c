#include "options.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "echod.h"
#include "ed_default_paths.h"
#include "valid.h"
#include "version.h"

options_t opt;

/* User for running the Echo daemon. */
#define DEF_DAEMON_USER "_echod"

#define DEF_BIND_ADDR "0.0.0.0"
#define DEF_ECHO_PORT 7
#define DEF_BACKLOG   32

static char const short_options[] =
  "C:" /* configuration file */
  "L:" /* log file */
  "P:" /* pid file */
  "S"  /* show configuration */
  "V"  /* version */
  "a:" /* local address to bind */
  "b:" /* tcp backlog queue limit */
  "f"  /* foreground mode */
  "h"  /* help */
  "p:" /* tcp port number to listen on */
  "u:" /* user identity to run as */
  ;

static struct option const long_options[] = {
  { "config",      required_argument, (int *) 0, 'C' },
  { "logfile",     required_argument, (int *) 0, 'L' },
  { "pidfile",     required_argument, (int *) 0, 'P' },
  { "show-config", no_argument,       (int *) 0, 'S' },
  { "version",     no_argument,       (int *) 0, 'V' },
  { "address",     required_argument, (int *) 0, 'a' },
  { "backlog",     required_argument, (int *) 0, 'b' },
  { "foreground",  no_argument,       (int *) 0, 'f' },
  { "help",        no_argument,       (int *) 0, 'h' },
  { "port",        required_argument, (int *) 0, 'p' },
  { "user",        required_argument, (int *) 0, 'u' },
  { (char *) 0,    no_argument,       (int *) 0,  0  }
};

static void usage(int status) {
  if (status) {
    fprintf(stderr, "Try '%s --help' for more information.\n", progname);
  } else {
    printf("Usage: %s [OPTIONS]...\n", progname);
    putchar('\n');

    fputs("Mandatory arguments to long options are mandatory for short options too.\n", stdout);
    putchar('\n');

    fputs("Options:\n", stdout);
    fputs("  -f, --foreground           run in the foreground\n", stdout);
    fputs("  -u, --user=<username>      run as user\n", stdout);
    fputs("  -C, --config=<file>        specify alternate configuration file\n", stdout);
    fputs("  -P, --pidfile=<file>       write process id to the specified file\n", stdout);
    fputs("  -L, --logfile=<file>       write log messages to the specified file\n", stdout);
    fputs("  -a, --address=<ip/domain>  bind to the specified address\n", stdout);
    fputs("  -p, --port=<number>        specify the port to listen on\n", stdout);
    fputs("  -b, --backlog=<number>     set the backlog queue limit\n", stdout);
    fputs("  -h, --help                 display this help and exit\n", stdout);
    fputs("  -V, --version              output version information and exit\n", stdout);
  }

  exit(status);
}

static void version(void) {
  printf("%s %s\n", progname, ECHOD_VERSION);
}

void init_options(void) {
  opt.detach = 1;
  opt.conffile = DEF_PATH_ECHODCONF;
  opt.pidfile = DEF_PATH_ECHODPID;
  opt.user = DEF_DAEMON_USER;
  opt.address = DEF_BIND_ADDR;
  opt.port = DEF_ECHO_PORT;
  opt.backlog = DEF_BACKLOG;
  opt.logfile = (char *) 0;
}

void preparse_args(int argc, char **argv) {
  int opt_char;

  /*
   * This separate getopt_long is needed to find the user config file
   * option ("--config") and parse it before the other user options.
   */
  for (;;) {
    opt_char = getopt_long(argc, argv, short_options, long_options, (int *) 0);
    if (opt_char == -1) {
      /* no more options */
      break;
    }

    switch (opt_char) {
    case 'C': /* --config */
      opt.conffile = optarg;
      break;

    default:
      break;
    }
  }
}

void parse_args(int argc, char **argv) {
  int opt_char, value;
  int show_config = 0;

  optind = 0;

  for (;;) {
    opt_char = getopt_long(argc, argv, short_options, long_options, (int *) 0);
    if (opt_char == -1) {
      /* no more options */
      break;
    }

    switch (opt_char) {
    case 'f':  /* --foreground */
      opt.detach = 0;
      break;

    case 'C':  /* --config */
      /* This option is already parsed in preparse_args(). */
      break;

    case 'L':  /* --logfile */
      opt.logfile = optarg;
      break;

    case 'P':  /* --pidfile */
      opt.pidfile = optarg;
      break;

    case 'u':  /* --user */
      opt.user = optarg;
      break;

    case 'a':  /* --address */
      opt.address = optarg;
      break;

    case 'p':  /* --port */
      value = atoi(optarg);
      if (!valid_port(value)) {
	fprintf(stderr, "%s: port number must be between 1 and 65535\n", progname);
        exit(1);
      }
      opt.port = value;
      break;

    case 'b':  /* --backlog */
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname);
	exit(1);
      }

      opt.backlog = value;
      break;

    /* undocumented -- dump CLI options for testing */
    case 'S':  /* --show-config */
      show_config = 1;
      break;

    case 'h':  /* --help */
      usage(0);
      break;

    case 'V':  /* --version */
      version();
      exit(0);
      break;

    default:
      usage(1);
    }
  }

  if (show_config) {
    fputs("Default paths:\n", stdout);
    fprintf(stdout, "  Config file: %s\n", DEF_PATH_ECHODCONF);
    fprintf(stdout, "  PID file:    %s\n", DEF_PATH_ECHODPID);
    fprintf(stdout, "\n *** %s configuration ***\n", progname);
    fprintf(stdout, "ConfigFile    = %s\n", opt.conffile);
    fprintf(stdout, "PIDFile       = %s\n", opt.pidfile);
    fprintf(stdout, "LogFile       = %s\n", opt.logfile);
    fprintf(stdout, "User          = %s\n", opt.user);
    fprintf(stdout, "Port          = %d\n", opt.port);
    fprintf(stdout, "Backlog       = %d\n", opt.backlog);
  }
}
