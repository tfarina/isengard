#include "wrapunix.h"

#include <sys/types.h>
#include <unistd.h>

pid_t Fork(void) {
  pid_t pid;

  if ((pid = fork()) == -1) {
    // err_sys("fork error");
  }

  return pid;
}

void Close(int fd) {
  if (close(fd) == -1) {
    // err_sys("close error");
  }
}
