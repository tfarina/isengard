#include "log.h"

#include <stdio.h>
#include <stdlib.h>

static void reportf(const char *prefix, const char *fmt, va_list args) {
    fflush(stderr);
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}

void fatal(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    reportf("fatal: ", fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

void error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    reportf("error: ", fmt, args);
    va_end(args);
}

void warning(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    reportf("warning: ", fmt, args);
    va_end(args);
}

void info(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    reportf("", fmt, args);
    va_end(args);
}
