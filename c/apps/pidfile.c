#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

/* SEE https://github.com/freebsd/freebsd/blob/master/lib/libutil/pidfile.c#L77 */
/* SEE http://git.savannah.gnu.org/cgit/tpop3d.git/tree/pidfile.c */

static int
pidfile_read(char const *path, pid_t *pid_out)
{
  int fd;
  char buf[16];
  ssize_t nbytes;
  char *endptr;

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    return errno;
  }

  nbytes = read(fd, buf, sizeof(buf) - 1);

  close(fd);

  if (nbytes < 0) {
    return errno;
  }

  buf[nbytes] = '\0';

  *pid_out = (pid_t) strtol(buf, &endptr, 10);
  if (*endptr != '\0') {
    return EINVAL;
  }

  return 0;
}

int main(void) {
  pid_t pid;

  pidfile_read("/var/run/crond.pid", &pid);

  printf("%ld\n", (long) pid);

  return 0;
}
