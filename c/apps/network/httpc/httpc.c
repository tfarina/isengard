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

#include "buffer.h"
#include "fnet.h"

#define MIN_BUFLEN 1024

static ssize_t fd_write(int fd, const char *buf, size_t len)
{
  ssize_t num_written; /* On success, the number of bytes written (sent). */

  do {
    num_written = write(fd, buf, len);
  } while (num_written == -1 && errno == EINTR);

  return num_written;
}

static ssize_t fd_write_all(int fd, const char *buf, size_t len)
{
  ssize_t total_written = 0; /* Number of total bytes written (sent). */

  while (len > 0) {
    ssize_t num_written; /* On success, the number of bytes written (sent). */

    num_written = fd_write(fd, buf, len);

    if (num_written < 0)
      return num_written;

    if (num_written == 0)
      break;

    printf("[DEBUG]: number of bytes sent: %zd\n", num_written);

    total_written += num_written;

    buf += num_written;
    len -= num_written;
  }

  return total_written;
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
    fprintf(stderr, "Failed to write the HTTP request");
    return -1;
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
