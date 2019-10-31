/*
 * From https://github.com/golang/go/blob/ce5cb037d171273f1a5294723234be5495c9d336/src/cmd/dist/unix.c#L817
 */

#include <stdio.h>

#include "cpuinfo.h"

int main(void)
{
        if (cpu_has_sse2()) {
                printf("CPU has SSE2 support.\n");
        } else {
                printf("CPU does not have SSE2 support.\n");
        }

        return 0;
}
