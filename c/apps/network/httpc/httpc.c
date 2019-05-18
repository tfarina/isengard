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

#include "fnet.h"
#include "macros.h"

#define MIN_BUFLEN 1024

typedef struct {
	char *data;
	size_t len;
	size_t cap;
} buffer_t;

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

static void *xrealloc(void *oldptr, size_t newsize)
{
        void *newptr;

	newptr = realloc(oldptr, newsize);
        if (newptr == NULL) {
                fatal("out of memory: %lu", newsize);
	}

	return newptr;
}

static void buffer_setlen(buffer_t *b, size_t len)
{
        b->len = len;
        b->data[len] = '\0';
}

static buffer_t *buffer_alloc(size_t capacity)
{
        buffer_t *b;

	b = (buffer_t *)malloc(sizeof(buffer_t));

        if (!b) {
	        return NULL;
	}

        b->data = malloc(capacity + 1);
        *b->data = 0; /* always 0 terminate data to allow string functions. */
        b->len = 0;
        b->cap = capacity;

        return b;
}

static void buffer_free(buffer_t *b)
{
	free(b->data);
	free(b);
}

static void buffer_grow(buffer_t *b, size_t extra)
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

static void buffer_append(buffer_t *b, const void *data, size_t len)
{
	buffer_grow(b, len);
	memcpy(b->data + b->len, data, len);
        buffer_setlen(b, b->len + len);
}

static ssize_t fd_write(int fd, const char *buf, size_t len)
{
  ssize_t rv;

  do {
    rv = write(fd, buf, len);
  } while (rv == -1 && errno == EINTR);

  return rv;
}

static ssize_t fd_write_all(int fd, const char *buf, size_t len)
{
  ssize_t total_bytes_sent = 0;

  while (len > 0) {
    ssize_t bytes_sent;

    bytes_sent = fd_write(fd, buf, len);

    if (bytes_sent < 0)
      return bytes_sent;

    if (bytes_sent == 0)
      break;

    printf("[DEBUG]: bytes sent: %zd\n", bytes_sent);

    total_bytes_sent += bytes_sent;

    buf += bytes_sent;
    len -= bytes_sent;
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
  const char *host;
  int port = 80;
  int sockfd;
  char request[MIN_BUFLEN];
  const char method[] = "GET";
  const char path[] = "/";
  size_t bytes_to_send;
  /*ssize_t bytes_received;*/
  /*char data[MIN_BUFLEN];*/
  buffer_t *response;
  char *buf, *p = NULL;
  size_t bufcap;
  ssize_t nbytes, nread = 0;

  if (argc != 2) {
    fprintf(stderr, "usage: %s url\n", argv[0]);
    return EXIT_FAILURE;
  }

  host = strdup(argv[1]);

  sockfd = fnet_tcp_socket_connect(host, port);

  sprintf(request, "%s %s HTTP/1.1\r\n"
	           "Host: %s\r\n"
	           "Connection: close\r\n"
                   "\r\n",
          method, path, host);

  bytes_to_send = strlen(request);

  printf("[DEBUG]: bytes to send: %zd\n", bytes_to_send);

  if (fd_write_all(sockfd, request, bytes_to_send) != bytes_to_send) {
    fatal("Failed to write the HTTP request");
  }

  printf("HTTP request sent, awaiting response...\n"),

  printf("[DEBUG]: receiving data...\n");

  response = buffer_alloc(102400);

  buf = response->data;
  bufcap = response->cap;

  while ((nbytes = fd_read(sockfd, buf + nread, bufcap - nread)) > 0) {
    printf("[DEBUG]: nbytes %zd nread %zd bufcap %zu\n", nbytes, nread, bufcap);
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

      printf("[DEBUG]: got header %zd bytes:\n%s\n\n", p - buf, buf);
    }

    printf("%s", buf);
  }

  /*
  while ((bytes_received = fd_read_all(sockfd, data, sizeof(data))) > 0) {
    buffer_append(response, data, bytes_received);
  }

  */
  printf("\n\n[DEBUG]: finished receiving data.\n\n");

  //printf("%s\n", response->data);

  buffer_free(response);

  return 0;
}
