#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

/* Get ip from domain name. */
static int hostname_to_ip(char *hostname, char *ip) {
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h;
  int rv;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(hostname, "http", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    h = (struct sockaddr_in *)p->ai_addr;
    strcpy(ip, inet_ntoa(h->sin_addr));
    printf("%s\n", ip);
  }

  freeaddrinfo(servinfo);

  return 0;
}

int main(int argc, char **argv) {
  char *hostname = NULL;
  char ip[100];

  if (argc < 2) {
    fprintf(stderr, "Please, provide a hostname to resolve\n");
    exit(EXIT_FAILURE);
  }

  hostname = argv[1];

  hostname_to_ip(hostname, ip);
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
