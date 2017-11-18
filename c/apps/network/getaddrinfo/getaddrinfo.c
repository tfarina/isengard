#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */

/* Get ip from domain name. */
static int hostname_to_ip(char *hostname, char *ipbuf, size_t ipbuf_len) {
  struct addrinfo hints, *addrlist, *cur;
  struct sockaddr_in *h;
  int rv;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(hostname, "http", &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    if (cur->ai_family == AF_INET) {
      h = (struct sockaddr_in *)cur->ai_addr;
      strcpy(ipbuf, inet_ntoa(h->sin_addr));
      printf("%s\n", ipbuf);
    }
  }

  freeaddrinfo(addrlist);

  return 0;
}

int main(int argc, char **argv) {
  char *hostname = NULL;
  char ip[NET_IP_STR_LEN];

  if (argc < 2) {
    fprintf(stderr, "Please, provide a hostname to resolve\n");
    exit(EXIT_FAILURE);
  }

  hostname = argv[1];

  hostname_to_ip(hostname, ip, sizeof(ip));
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
