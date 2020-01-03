#include "ec_log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void vprintf_msg(const char *prefix, const char *fmt, va_list ap) {
    fflush(stderr);
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
}

void EC_NORETURN fatal(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("fatal: ", fmt, ap);
    va_end(ap);

    exit(1);
}

void error(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("error: ", fmt, ap);
    va_end(ap);
}

void warn(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("warning: ", fmt, ap);
    va_end(ap);
}

void info(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("", fmt, ap);
    va_end(ap);
}
