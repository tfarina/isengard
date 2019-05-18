#include <stdio.h>
#include <stdlib.h>

#include "fnet.h"

#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */

int main(int argc, char **argv) {
  char *hostname = NULL;
  char ip[NET_IP_STR_LEN];

  if (argc < 2) {
    fprintf(stderr, "Please, provide a hostname to resolve\n");
    exit(EXIT_FAILURE);
  }

  hostname = argv[1];

  fnet_resolve(NULL, hostname, ip, sizeof(ip));
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
