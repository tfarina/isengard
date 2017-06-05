#include <stdio.h>
#include <unistd.h>

static int cpu_get_count(void)
{
  return (int)sysconf(_SC_NPROCESSORS_ONLN);
}

int main(int argc, char **argv)
{
  printf("CPU count: %d\n", cpu_get_count());

  return 0;
}
