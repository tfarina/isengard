#include "cpuinfo.h"

#include <unistd.h>

static void __cpuid(int dst[4], int ax)
{
#if defined(__GNUC__) && defined(__i386__)
        __asm__ volatile("mov %%ebx, %%edi\n\t"
                         "cpuid\n\t"
                         "xchgl %%ebx, %%edi"
                        : "=a" (dst[0]),
                          "=D" (dst[1]),
                          "=c" (dst[2]),
                          "=d" (dst[3])
                        : "0" (ax));
#elif defined(__GNUC__) && defined(__x86_64__)
        asm volatile("cpuid"
                   : "=a" (dst[0]),
	             "=b" (dst[1]),
                     "=c" (dst[2]),
                     "=d" (dst[3])
                   : "0" (ax));
#else
        dst[0] = dst[1] = dst[2] = dst[3] = 0;
#endif
}

int
cpu_has_sse2(void)
{
        int info[4];

        __cpuid(info, 1);
        return (info[3] & (1 << 26)) != 0;
}

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
