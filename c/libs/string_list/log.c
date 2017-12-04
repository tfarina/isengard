#include "log.h"

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void vwrite(const char *msg, va_list ap)
{
        char *fmt;

        if (asprintf(&fmt, "%s\n", msg) == -1)
	        exit(1);

        if (vfprintf(stderr, fmt, ap) == -1)
                exit(1);

        fflush(stderr);
        free(fmt);
}

static void vfatal(const char *msg, va_list ap)
{
        char *fmt;

        if (asprintf(&fmt, "fatal: %s", msg) == -1)
                exit(1);

        vwrite(fmt, ap);

        free(fmt);

	exit(1);
}

void fatal(const char *msg, ...)
{
        va_list args;

        va_start(args, msg);

	vfatal(msg, args);
}
