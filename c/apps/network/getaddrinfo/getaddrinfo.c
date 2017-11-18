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
#define FNET_ERR_LEN 256

static void fnet_set_error(char *err, const char *fmt, ...) {
  va_list ap;

  if (!err) {
    return;
  }

  va_start(ap, fmt);
  vsnprintf(err, FNET_ERR_LEN, fmt, ap);
  va_end(ap);
}

/**
 * Resolves hostname to IP address.
 *
 * @return int -1 on error, 0 on success.
 */
static int net_resolve(char *err, char *hostname, char *ipbuf, size_t ipbuf_len) {
  struct addrinfo hints, *addrlist;
  int rv;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(hostname, "http", &hints, &addrlist)) != 0) {
    fnet_set_error(err, "%s", gai_strerror(rv));
    return FNET_ERR;
  }

  if (addrlist->ai_family == AF_INET) {
    struct sockaddr_in *sa = (struct sockaddr_in *)addrlist->ai_addr;
    inet_ntop(AF_INET, &(sa->sin_addr), ipbuf, ipbuf_len);
  } else {
    struct sockaddr_in6 *sa = (struct sockaddr_in6 *)addrlist->ai_addr;
    inet_ntop(AF_INET6, &(sa->sin6_addr), ipbuf, ipbuf_len);
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

  net_resolve(NULL, hostname, ip, sizeof(ip));
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
