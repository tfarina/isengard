#include "ed_cmdline.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ed_default_paths.h"
#include "ed_globals.h"
#include "ed_version.h"
#include "valid.h"

static char const short_options[] =
  "h"  /* help */
  "V"  /* version */
  "f"  /* foreground mode */
  "C:" /* configuration file */
  "L:" /* log file */
  "P:" /* pid file */
  "u:" /* user identity to run as */
  "a:" /* local address to bind */
  "p:" /* tcp port number to listen on */
  "b:" /* tcp backlog queue limit */
  "S"  /* show configuration */
  ;

static struct option const long_options[] = {
  { "help",        no_argument,       (int *) 0, 'h' }, /* help */
  { "version",     no_argument,       (int *) 0, 'V' }, /* version */
  { "foreground",  no_argument,       (int *) 0, 'f' }, /* foreground mode */
  { "config",      required_argument, (int *) 0, 'C' }, /* configuration file */
  { "logfile",     required_argument, (int *) 0, 'L' }, /* log file */
  { "pidfile",     required_argument, (int *) 0, 'P' }, /* pid file */
  { "user",        required_argument, (int *) 0, 'u' }, /* user identity to run as */
  { "address",     required_argument, (int *) 0, 'a' }, /* local address to bind */
  { "port",        required_argument, (int *) 0, 'p' }, /* tcp port number to listen on */
  { "backlog",     required_argument, (int *) 0, 'b' }, /* tcp backlog queue limit */
  { "show-config", no_argument,       (int *) 0, 'S' }, /* show configuration */
  { (char *) 0,    no_argument,       (int *) 0,  0  }
};

static void usage(int status);

static void version(void) {
  printf("%s %s\n", progname_g, ED_VERSION_STR);
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
	fprintf(stderr, "%s: option -p requires a non zero number\n", progname_g);
	exit(1);
      }
      if (!valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", progname_g, value);
        exit(1);
      }

      opt->port = value;
      break;

    case 'b':  /* --backlog */
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", progname_g);
	exit(1);
      }

      opt->backlog = value;
      break;

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
    fprintf(stdout, "\n *** %s configuration ***\n", progname_g);
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
    fprintf(stderr, "Try '%s --help' for more information.\n", progname_g);
  } else {
    printf("Usage: %s [OPTION]...\n", progname_g);
    putc('\n', stdout);

    fputs("Mandatory arguments to long options are mandatory for short options too.\n", stdout);
    putc('\n', stdout);

    fputs("Options:\n", stdout);
    fputs("  -C, --config=FILE          read configuration from the specified file\n", stdout);
    fputs("  -P, --pidfile=FILE         write process id to the specified file\n", stdout);
    putc('\n', stdout);

    fputs("  -u, --user=NAME            run the daemon as user\n", stdout);
    putc('\n', stdout);

    fputs("  -a, --address=ADDRESS      bind to the specified address\n", stdout);
    fputs("  -p, --port=VALUE           TCP port to listen on\n", stdout);
    fputs("  -b, --backlog=VALUE        set the backlog queue limit\n", stdout);
    putc('\n', stdout);

    fputs("  -f, --foreground           run the daemon in the foreground\n", stdout);
    putc('\n', stdout);

    fputs("  -L, --logfile=FILE         write log messages to the specified file\n", stdout);
    putc('\n', stdout);

    fputs("  -h, --help                 display this help and exit\n", stdout);
    fputs("  -V, --version              output version information and exit\n", stdout);
  }

  exit(status);
}
