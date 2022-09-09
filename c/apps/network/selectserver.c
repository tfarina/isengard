/*
 * Author: beej.us
 * URL: http://beej.us/guide/bgnet/examples/selectserver.c
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT "9034"

static void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

int main(int argc, char **argv) {
  fd_set master_read_fd_set; /* master file descriptor list. */
  fd_set work_read_fd_set; /* temp file descriptor list for select. */
  int maxfd; /* maximum file descriptor number. */

  int listener; /* listening socket descriptor. */
  int newfd; /* newly accepted socket descriptor. */
  struct sockaddr_storage remoteaddr; /* client address. */
  socklen_t addrlen;

  char buf[256]; /* buffer for client data. */
  int nbytes;

  char remote_ip[INET6_ADDRSTRLEN];

  int yes = 1; /* for setsockopt() SO_REUSEADDR below. */
  int i, j, rv;

  struct addrinfo hints, *ai, *p;

  FD_ZERO(&master_read_fd_set); /* clear the master and temp sets. */
  FD_ZERO(&work_read_fd_set);

  /* Get us a socket and bind it. */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
    fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  /* Loop through all the results and bind to the first we can. */
  for (p = ai; p != NULL; p = p->ai_next) {
    listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (listener == -1) {
      continue;
    }

    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
      close(listener);
      continue;
    }

    break;
  }

  /* If we got here, it means we didn't get bound. */
  if (p == NULL) {
    fprintf(stderr, "selectserver: failed to bind. \n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(ai); /* All done with this. */

  if (listen(listener, 10) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* Add the listener to the master set. */
  FD_SET(listener, &master_read_fd_set);

  /* Keep track of the biggest file descriptor. */
  maxfd = listener; /* So far, it's this one. */

  for (;;) {
    work_read_fd_set = master_read_fd_set; /* Make a copy. */

    if (select(maxfd + 1, &work_read_fd_set, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    /* Run through the existing connections looking for data to read. */
    for (i = 0; i <= maxfd; i++) {
      if (FD_ISSET(i, &work_read_fd_set)) { /* We got one. */
        if (i == listener) {
          /* Handle new connections. */
          addrlen = sizeof(remoteaddr);
          if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
            perror("accept");
          }
        } else {
          FD_SET(newfd, &master_read_fd_set); /* Add to master set. */
          if (newfd > maxfd) { /* Keep track of the max. */
            maxfd = newfd;
          }
          printf("selectserver: new connection from %s on "
                 "socket %d\n", inet_ntop(remoteaddr.ss_family,
                         get_in_addr((struct sockaddr *)&remoteaddr), remote_ip, INET6_ADDRSTRLEN), newfd);
        }
      } else {
        /* Handle data from a client. */
        if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
          /* Got error or connection was closed by client. */
          if (nbytes == 0) {
            /* Connection was closed. */
            printf("selectserver: socket %d hung up\n", i);
          } else {
            perror("recv");
          }
          close(i);
          FD_CLR(i, &master_read_fd_set); /* Remove from master set. */
        } else {
          /* We got some data from a client. */
          for (j = 0; j <= maxfd; j++) {
            /* Send to everyone. */
            if (FD_ISSET(j, &master_read_fd_set)) {
              /* Except the listener and ourselves. */
              if (j != listener && j != i) {
                if (send(j, buf, nbytes, 0) == -1) {
                  perror("send");
                }
              }
            }
          }
        }
      }
    }
  }

  return 0;
}
