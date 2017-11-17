/*
 * Heavily modified from https://github.com/reagent/http
 */

#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MIN_BUFLEN 1024

typedef struct {
	char *data;
	size_t len;
	size_t cap;
} sbuf;

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

static NORETURN void fatal(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);

        fprintf(stderr, "fatal: ");
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");

        va_end(args);

        exit(EXIT_FAILURE);
}

static void *xrealloc(void *p, size_t n)
{
	p = realloc(p, n);
        if (p == NULL)
                fatal("out of memory: %lu", n);
	return p;
}

static void sbuf_setlen(sbuf *b, size_t len)
{
        b->len = len;
        b->data[len] = '\0';
}

static sbuf *sbuf_alloc(size_t capacity)
{
        sbuf *b = malloc(sizeof(sbuf));

        b->data = malloc(capacity + 1);
        *b->data = 0; /* always 0 terminate data to allow string functions. */
        b->len = 0;
        b->cap = capacity;

        return b;
}

static void sbuf_init(sbuf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

static void sbuf_free(sbuf *b)
{
	free(b->data);
	sbuf_init(b);
}

static void sbuf_grow(sbuf *b, size_t extra)
{
	size_t want;

        want = b->len + extra;
        if (want > b->cap) {
                b->cap = 2 * want;
                if (b->cap < 64)
                        b->cap = 64;
                b->data = xrealloc(b->data, b->cap);
        }
}

static void sbuf_append(sbuf *b, const void *data, size_t len)
{
	sbuf_grow(b, len);
	memcpy(b->data + b->len, data, len);
        sbuf_setlen(b, b->len + len);
}

static ssize_t fd_write(int fd, char *buf, size_t len)
{
  ssize_t rv;

  do {
    rv = write(fd, buf, len);
  } while (rv == -1 && errno == EINTR);

  return rv;
}

static ssize_t fd_write_all(int fd, char *buf, size_t len)
{
  ssize_t bytes_sent, total_bytes_sent = 0;

  while (len) {
    bytes_sent = fd_write(fd, buf, len);
    if (bytes_sent <= 0)
      return -1;

    printf("# sent %zd bytes\n", bytes_sent);

    buf += bytes_sent;
    len -= bytes_sent;
    total_bytes_sent += bytes_sent;
  }

  return total_bytes_sent;
}

static ssize_t fd_read(int fd, char *buf, size_t len)
{
  ssize_t rv;

  do {
    rv = read(fd, buf, len);
  } while (rv == -1 && errno == EINTR);

  return rv;
}

static ssize_t fd_read_all(int fd, char *buf, size_t len)
{
  char *b = buf;

  while (len) {
    ssize_t bytes_received;

    bytes_received = fd_read(fd, b, len);
    if (bytes_received < 0)
      return -1;
    if (bytes_received == 0)
      break;

    b += bytes_received;
    len -= bytes_received;
  }

  return b - buf;
}

int main(int argc, char **argv) {
  int rv;
  const char host[] = "reddit.com";
  int port = 80;
  char portstr[6];  /* strlen("65535") + 1; */
  struct addrinfo hints, *addrlist, *cur;
  int sockfd;
  char request[MIN_BUFLEN];
  const char method[] = "GET";
  size_t bytes_to_send;
  ssize_t bytes_received;
  char data[MIN_BUFLEN];
  sbuf *response;

  snprintf(portstr, sizeof(portstr), "%d", port);
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  printf("Resolving %s...\n", host);

  if ((rv = getaddrinfo(host, portstr, &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  /* Loop through all the results and connect to the first we can. */
  for (cur = addrlist; cur != NULL; cur = cur->ai_next) {
    char addr[NI_MAXHOST], strport[NI_MAXSERV];
    if ((rv = getnameinfo(cur->ai_addr, cur->ai_addrlen,
                          addr, sizeof(addr), strport, sizeof(strport),
                          NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      fatal("getnameinfo failed: %.100s", gai_strerror(rv));
    }
    printf("Connecting to %.200s port %s.\n", addr, strport);

    if ((sockfd = socket(cur->ai_family, cur->ai_socktype,
                         cur->ai_protocol)) == -1) {
      break;
    }

    if (connect(sockfd, cur->ai_addr, cur->ai_addrlen) == 0) {
      break;
    }

    close(sockfd);
    sockfd = -1;
  }

  if (sockfd == -1) {
    fprintf(stderr, "Failed to connect to %s\n", host);
    freeaddrinfo(addrlist);
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(addrlist);

  printf("Connection established.\n");

  sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
          method, "/", host);

  bytes_to_send = strlen(request);

  printf("bytes to send: %ld\n", bytes_to_send);

  if (fd_write_all(sockfd, request, strlen(request)) == -1)
    fatal("Failed to write the HTTP request");

  printf("HTTP request sent, awaiting response...\n"),

  printf("receiving data...\n");

  response = sbuf_alloc(102400);

  char *buf, *p = NULL;
  size_t bufcap;
  ssize_t nbytes, nread = 0;

  buf = response->data;
  bufcap = response->cap;

  while ((nbytes = fd_read(sockfd, buf + nread, bufcap - nread)) > 0) {
    printf("nbytes %zd nread %zd bufcap %zu\n", nbytes, nread, bufcap);
    nread += nbytes;
    buf[nread] = 0;

    if (nread < 4) {
      continue;
    }

    if (nread == nbytes) {
      p = buf;
    } else {
      p = buf + nread - nbytes - 3;
    }

    if ((p = strstr(p, "\r\n\r\n"))) {
      /* found end-of-header */
      *p = 0;

      printf("# got header %zd bytes:\n%s\n\n", p - buf, buf);
    }

    printf("%s", buf);
  }

  /*
  while ((bytes_received = fd_read_all(sockfd, data, sizeof(data))) > 0) {
    sbuf_append(response, data, bytes_received);
  }

  */
  printf("finished receiving data.\n\n");

  //printf("%s\n", response->data);

  sbuf_free(response);

  return 0;
}
