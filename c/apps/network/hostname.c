#include "hostname.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif

char *
get_hostname(void)
{
  static char buf[MAXHOSTNAMELEN + 1];

  if (gethostname(buf, sizeof(buf)) < 0) {
    fprintf(stderr, "gethostname() failed: %s\n", strerror(errno));
    return NULL;
  }

  buf[MAXHOSTNAMELEN] = '\0'; /* Null terminate, just to be safe. */

  return buf;
}
