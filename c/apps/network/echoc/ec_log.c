#include "ec_log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void vprintf_msg(const char *prefix, const char *fmt, va_list args) {
    fflush(stderr);
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}

void fatal(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf_msg("fatal: ", fmt, args);
    va_end(args);

    exit(1);
}

void error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf_msg("error: ", fmt, args);
    va_end(args);
}

void warn(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf_msg("warning: ", fmt, args);
    va_end(args);
}

void info(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf_msg("", fmt, args);
    va_end(args);
}
