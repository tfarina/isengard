#include "fnet.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static void fnet_set_error(char *err, const char *fmt, ...)
{
  va_list ap;

  if (!err) {
    return;
  }

  va_start(ap, fmt);
  vsnprintf(err, FNET_ERR_LEN, fmt, ap);
  va_end(ap);
}

int fnet_resolve(char *err, char *hostname, char *ipbuf, size_t ipbuf_len)
{
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
