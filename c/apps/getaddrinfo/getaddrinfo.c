#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

/* Get ip from domain name. */
static int hostname_to_ip(char *hostname, char *ip)
{
  int sockfd;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *p;
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

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Please, provide a hostname to resolve\n");
    exit(1);
  }

  char *hostname = argv[1];
  char ip[100];

  hostname_to_ip(hostname, ip);
  printf("%s resolved to %s\n", hostname, ip);

  return 0;
}
