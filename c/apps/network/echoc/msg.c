#include "msg.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void vprintf_msg(const char *prefix, const char *fmt, va_list ap) {
    fflush(stderr);
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
}

/**
 * Report an error message and terminate.
 */
void EC_NORETURN fatal(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("fatal: ", fmt, ap);
    va_end(ap);

    exit(1);
}

/**
 * Report an error message.
 */
void error(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("error: ", fmt, ap);
    va_end(ap);
}

/**
 * Report an warning message.
 */
void warn(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("warning: ", fmt, ap);
    va_end(ap);
}

/**
 * Report an informative message.
 */
void info(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf_msg("", fmt, ap);
    va_end(ap);
}
