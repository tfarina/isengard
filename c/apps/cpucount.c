#include <stdio.h>

#include "cpuinfo.h"

int main(int argc, char **argv)
{
  printf("CPU count: %d\n", cpu_get_count());

  return 0;
}
