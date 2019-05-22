/*
 * showip.c - Show IP addresses for a given host.
 *
 * Slightly modified from beej.us/guide/bgnet/examples/showip.c
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  char *hostname;
  struct addrinfo hints, *addrlist, *cur;
  int ret;
  char ipstr[INET6_ADDRSTRLEN];

  if (argc != 2) {
    fprintf(stderr, "usage: showip hostname\n");
    exit(EXIT_FAILURE);
  }

  hostname = strdup(argv[1]);

  printf("IP addresses for %s:\n\n", hostname);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; /* AF_INET or AF_INET6 */
  hints.ai_socktype = SOCK_STREAM;

  if ((ret = getaddrinfo(hostname, NULL, &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    void *addr;
    char *ipver;

    /* Get the pointer to the address. */
    if (cur->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)cur->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)cur->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    /* Convert the IP to a string and print it. */
    inet_ntop(cur->ai_family, addr, ipstr, sizeof(ipstr));
    printf("%s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(addrlist);  /* We are done with this structure. */

  return 0;
}
