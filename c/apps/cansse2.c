/*
 * From https://github.com/golang/go/blob/ce5cb037d171273f1a5294723234be5495c9d336/src/cmd/dist/unix.c#L817
 */

#include <stdio.h>

static void
__cpuid(int dst[4], int ax)
{
#ifdef __i386__
        asm volatile(
               "mov %%ebx, %%edi\n\t"
                "cpuid\n\t"
                "xchgl %%ebx, %%edi"
                : "=a" (dst[0]), "=D" (dst[1]), "=c" (dst[2]), "=d" (dst[3])
                : "0" (ax));
#elif defined(__x86_64__)
        asm volatile("cpuid"
                : "=a" (dst[0]), "=b" (dst[1]), "=c" (dst[2]), "=d" (dst[3])
                : "0" (ax));
#else
        dst[0] = dst[1] = dst[2] = dst[3] = 0;
#endif
}

int cansse2(void)
{
        int info[4];

        __cpuid(info, 1);
        return (info[3] & (1<<26)) != 0;        // SSE2
}

int main(int argc, char **argv)
{
        if (cansse2())
          printf("SSE2 support\n");
        else
          printf("No SSE2 support\n");

        return 0;
}
