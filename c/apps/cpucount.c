#include <stdio.h>
#include <unistd.h>

static int cpu_get_count(void)
{
  int cpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
  if (cpus < 0) {
    return 0;
  }

  return cpus;
}

int main(int argc, char **argv)
{
  printf("CPU count: %d\n", cpu_get_count());

  return 0;
}
