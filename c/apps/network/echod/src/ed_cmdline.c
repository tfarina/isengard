#include "ed_cmdline.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ed_globals.h"
#include "ed_rcode.h"
#include "ed_validate.h"
#include "ed_version.h"

static void print_help(int status);

static void print_version(void) {
  printf("%s %s\n", ed_g_progname, ED_VERSION_STR);
}

void ed_cmdline_parse(int argc, char **argv, ed_config_t *config) {
  int opt_char, value;

  static char const short_options[] =
    "h"  /* help */
    "V"  /* version */
    "d"  /* daemon mode */
    "c:" /* configuration file */
    "L:" /* log file */
    "P:" /* pid file */
    "u:" /* user identity to run as */
    "a:" /* local address to bind */
    "p:" /* tcp port number to listen on */
    "b:" /* tcp backlog queue limit */
    "S"  /* show configuration */
    ;

  static struct option const long_options[] = {
    { "help",        no_argument,       NULL, 'h' }, /* help */
    { "version",     no_argument,       NULL, 'V' }, /* version */
    { "daemonize",   no_argument,       NULL, 'd' }, /* daemon mode */
    { "conffile",    required_argument, NULL, 'c' }, /* configuration file */
    { "logfile",     required_argument, NULL, 'L' }, /* log file */
    { "pidfile",     required_argument, NULL, 'P' }, /* pid file */
    { "user",        required_argument, NULL, 'u' }, /* user identity to run as */
    { "address",     required_argument, NULL, 'a' }, /* local address to bind */
    { "port",        required_argument, NULL, 'p' }, /* tcp port number to listen on */
    { "backlog",     required_argument, NULL, 'b' }, /* tcp backlog queue limit */
    { "show-config", no_argument,       NULL, 'S' }, /* show configuration */
    { NULL,          0,                 NULL,  0  }
  };

  int show_config = 0;

  for (;;) {
    opt_char = getopt_long(argc, argv, short_options, long_options, NULL);
    if (opt_char == -1) {
      /* no more options */
      break;
    }

    switch (opt_char) {
    case 'd':
      config->daemonize = 1;
      break;

    case 'c':
      config->conffile = optarg;
      break;

    case 'L':
      config->logfile = optarg;
      break;

    case 'P':
      config->pidfile = optarg;
      break;

    case 'u':
      config->username = optarg;
      break;

    case 'a':
      config->address = optarg;
      break;

    case 'p':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", ed_g_progname);
	exit(1);
      }
      if (!ed_valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", ed_g_progname, value);
        exit(1);
      }

      config->port = value;
      break;

    case 'b':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", ed_g_progname);
	exit(1);
      }

      config->backlog = value;
      break;

    case 'S':
      show_config = 1;
      break;

    case 'h':
      print_help(0);
      break;

    case 'V':
      print_version();
      exit(0);
      break;

    default:
      print_help(1);
    }
  }

  if (show_config) {
    fprintf(stdout, " *** %s configuration ***\n", ed_g_progname);
    fprintf(stdout, "username  = %s\n", config->username);
    fprintf(stdout, "conffile  = %s\n", config->conffile);
    fprintf(stdout, "pidfile   = %s\n", config->pidfile);
    fprintf(stdout, "logfile   = %s\n", config->logfile);
    fprintf(stdout, "port      = %d\n", config->port);
    fprintf(stdout, "backlog   = %d\n", config->backlog);
  }
}

static void print_help(int status) {
  if (status) {
    fprintf(stderr, "Try '%s --help' for more information.\n", ed_g_progname);
  } else {
    printf("Usage: %s [OPTION]...\n\n", ed_g_progname);

    fputs("Mandatory arguments to long options are mandatory for short options too.\n\n", stdout);

    fputs("Options:\n", stdout);
    fputs("  -d, --daemonize            fork and detach from controlling terminal (background mode)\n", stdout);
    fputs("  -c, --conffile=FILE        read configuration from the specified file\n", stdout);
    fputs("  -L, --logfile=FILE         write log messages to the specified file\n", stdout);
    fputs("  -P, --pidfile=FILE         write process id to the specified file\n", stdout);
    fputs("  -u, --user=NAME            user identity to run as\n", stdout);
    fputs("  -a, --address=ADDRESS      bind to the specified address\n", stdout);
    fputs("  -p, --port=VALUE           set the tcp port to listen on\n", stdout);
    fputs("  -b, --backlog=VALUE        the backlog argument of listen() applied to the\n", stdout);
    fputs("                             listening socket\n", stdout);
    fputs("  -h, --help                 display this help and exit\n", stdout);
    fputs("  -V, --version              output version information and exit\n", stdout);
  }

  exit(status);
}
