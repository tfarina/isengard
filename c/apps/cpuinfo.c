#include "cpuinfo.h"

#include <unistd.h>

int
cpu_get_count(void)
{
  int cpus;

  cpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
  if (cpus < 0) {
    return 0;
  }

  return cpus;
}
