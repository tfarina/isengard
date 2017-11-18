#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */

#define FNET_OK 0
#define FNET_ERR -1

/**
 * Resolves hostname to IP address.
 *
 * @return int -1 on error, 0 on success.
 */
static int net_resolve(char *hostname, char *ipbuf, size_t ipbuf_len) {
  struct addrinfo hints, *addrlist, *cur;
  int rv;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(hostname, "http", &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return FNET_ERR;
  }

  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if (cur->ai_family == AF_INET) {
      struct sockaddr_in *sa = (struct sockaddr_in *)cur->ai_addr;
      inet_ntop(AF_INET, &(sa->sin_addr), ipbuf, ipbuf_len);
      printf("%s\n", ipbuf);
    } else {
      struct sockaddr_in6 *sa = (struct sockaddr_in6 *)cur->ai_addr;
      inet_ntop(AF_INET6, &(sa->sin6_addr), ipbuf, ipbuf_len);
      printf("%s\n", ipbuf);
    }
  }

  freeaddrinfo(addrlist);

  return FNET_OK;
}

int main(int argc, char **argv) {
  char *hostname = NULL;
  char ip[NET_IP_STR_LEN];

  if (argc < 2) {
    fprintf(stderr, "Please, provide a hostname to resolve\n");
    exit(EXIT_FAILURE);
  }

  hostname = argv[1];

  net_resolve(hostname, ip, sizeof(ip));
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
