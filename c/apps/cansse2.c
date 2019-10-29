/*
 * From https://github.com/golang/go/blob/ce5cb037d171273f1a5294723234be5495c9d336/src/cmd/dist/unix.c#L817
 */

#include <stdio.h>

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

/**
 * Returns true if the CPU has SSE2 features.
 */
static int cpu_has_sse2(void)
{
        int info[4];

        __cpuid(info, 1);
        return (info[3] & (1 << 26)) != 0;
}

int main(void)
{
        if (cpu_has_sse2()) {
                printf("CPU has SSE2 support.\n");
        } else {
                printf("CPU does not have SSE2 support.\n");
        }

        return 0;
}
