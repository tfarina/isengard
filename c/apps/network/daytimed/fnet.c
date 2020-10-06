#include "fnet.h"

#include <errno.h>
#include <string.h>
#include <sys/socket.h>

#include "log.h"

int fnet_set_reuseaddr(int fd)
{
  int reuse = 1;

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    error("setsockopt SO_REUSEADDR: %s", strerror(errno));
    return -1;
  }

  return 0;
}
