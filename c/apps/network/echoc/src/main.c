/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "fnet.h"
#include "msg.h"
#include "os_path.h"
#include "validate.h"

#define BUFSIZE 4096

#define DEF_ECHO_PORT 7

static char const *progname;

static char const short_options[] =
    "h"   /* help */
    ;

static struct option const long_options[] = {
  { "help",        no_argument,       (int *) 0, 'h' },
  { (char *) 0,    no_argument,       (int *) 0,  0  }
};

static void usage(int status) {
  if (status) {
    fprintf(stderr, "Try '%s --help' for more information.\n", progname);
  } else {
    printf("Usage: %s [OPTIONS] host [port]\n", progname);
    putchar('\n');

    fputs("Options:\n", stdout);
    fputs("  -h, --help              display this help and exit\n", stdout);
  }

  exit(status);
}

int main(int argc, char **argv) {
  int optchr, value, sockfd, err;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];
  char *host;
  int port;

  progname = os_path_basename(*(argv + 0));

  port = DEF_ECHO_PORT;

  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, NULL);
    if (optchr == -1) {
      /* no more options */
      break;
    }

    switch (optchr) {
    case 'h':  /* --help */
      usage(0);

    case '?':
      /* Consider invalid option as fatal. e.g. echoc -R */
      usage(1);

    default:
      /* An unrecognized option is considered a fatal error. */
      fatal("Unrecognized option.");
    }
  }

  /* Takes out the program name from the argument array. */
  argc -= optind;
  argv += optind;

  /* It should have at least the destination address or hostname of the server. */
  if (argc < 1) {
    usage(0);
  }

  host = argv[0];

  /* Looks like we have a port argument, otherwise let's just keep going with the default port (7). */
  if (argc > 1) {
    value = atoi(argv[1]);
    if (value <= 0) {
      fprintf(stderr, "%s: port requires a non zero number\n", progname);
      return EXIT_FAILURE;
    }
    if (!valid_port(value)) {
      fprintf(stderr, "%s: %d is not a valid port\n", progname, value);
      return EXIT_FAILURE;
    }
    port = value;
  }

  err = fnet_tcp_socket_connect(host, port, &sockfd);
  if (err < 0) {
    return -1;
  }

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  while (1) {
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);

    read(sockfd, recvline, BUFSIZE);
    fputs(recvline, stdout);
  }

  return 0;
}
