#include "ed_cmdline.h"

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
#include "ed_version.h"
#include "valid.h"

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

static void usage(int status);

static void version(void) {
  printf("%s %s\n", progname, ED_VERSION_STR);
}

void preparse_args(int argc, char **argv, options_t *opt) {
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
      opt->conffile = optarg;
      break;

    default:
      break;
    }
  }
}

void parse_args(int argc, char **argv, options_t *opt) {
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
      opt->detach = 0;
      break;

    case 'C':  /* --config */
      /* This option is already parsed in preparse_args(). */
      break;

    case 'L':  /* --logfile */
      opt->logfile = optarg;
      break;

    case 'P':  /* --pidfile */
      opt->pidfile = optarg;
      break;

    case 'u':  /* --user */
      opt->user = optarg;
      break;

    case 'a':  /* --address */
      opt->address = optarg;
      break;

    case 'p':  /* --port */
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", progname);
	exit(1);
      }
      if (!valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", progname, value);
        exit(1);
      }

      opt->port = value;
      break;

    case 'b':  /* --backlog */
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname);
	exit(1);
      }

      opt->backlog = value;
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
    fprintf(stdout, "config    = %s\n", opt->conffile);
    fprintf(stdout, "pidfile   = %s\n", opt->pidfile);
    fprintf(stdout, "user      = %s\n", opt->user);
    fprintf(stdout, "port      = %d\n", opt->port);
    fprintf(stdout, "backlog   = %d\n", opt->backlog);
    fprintf(stdout, "logfile   = %s\n", opt->logfile);
  }
}

static void usage(int status) {
  if (status) {
    fprintf(stderr, "Try '%s --help' for more information.\n", progname);
  } else {
    printf("Usage: %s [OPTIONS]...\n", progname);
    putc('\n', stdout);

    fputs("Mandatory arguments to long options are mandatory for short options too.\n", stdout);
    putc('\n', stdout);

    fputs("Options:\n", stdout);
    fputs("  -C, --config=<file>        specify an alternative configuration file\n", stdout);
    fputs("  -P, --pidfile=<file>       write process id to the specified file\n", stdout);
    putc('\n', stdout);

    fputs("  -u, --user=<username>      run the daemon as user\n", stdout);
    putc('\n', stdout);

    fputs("  -a, --address=<ip/domain>  bind to the specified address\n", stdout);
    fputs("  -p, --port=<number>        TCP port to listen on\n", stdout);
    fputs("  -b, --backlog=<number>     set the backlog queue limit\n", stdout);
    putc('\n', stdout);

    fputs("  -f, --foreground           run the daemon in the foreground\n", stdout);
    putc('\n', stdout);

    fputs("  -L, --logfile=<file>       write log messages to the specified file\n", stdout);
    putc('\n', stdout);

    fputs("  -h, --help                 display this help and exit\n", stdout);
    fputs("  -V, --version              output version information and exit\n", stdout);
  }

  exit(status);
}
