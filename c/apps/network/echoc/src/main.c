/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

/* config.h must be included first.*/
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "check.h"
#include "echoc.h"
#include "fnet.h"
#include "msg.h"
#include "options.h"
#include "os_path.h"

#define BUFSIZE 4096

#define DEF_ECHO_PORT 7

char const *progname;

int main(int argc, char **argv) {
  int tmp_port, sockfd, err;
  char *host;
  int port;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];

  progname = os_path_basename(*(argv + 0));

  port = DEF_ECHO_PORT;

  parse_args(argc, argv);

  host = argv[1];

  /* Looks like we have a port argument, otherwise let's just keep going with the default port (7). */
  if (argc > 1) {
    tmp_port = atoi(argv[2]);
    if (tmp_port <= 0) {
      fprintf(stderr, "%s: port requires a non zero number\n", progname);
      return EXIT_FAILURE;
    }
    if (!is_valid_port(tmp_port)) {
      fprintf(stderr, "%s: %d is not a valid port\n", progname, tmp_port);
      return EXIT_FAILURE;
    }
    port = tmp_port;
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
