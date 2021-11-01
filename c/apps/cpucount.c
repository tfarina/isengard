#include <stdio.h>

#include "cpu.h"

int main(int argc, char **argv)
{
  printf("CPU count: %d\n", cpu_get_count());

  return 0;
}
