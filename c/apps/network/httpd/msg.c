#include "msg.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void vmsg(char const *prefix, char const *fmt, va_list ap) {
    fflush(stderr);
    fputs(prefix, stderr);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
}

/**
 * Prints an error message on stderr, then terminates with EXIT_FAILURE.
 */
MK_NORETURN void fatal(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("fatal: ", fmt, ap);
    va_end(ap);

    exit(1);
}

/**
 * Prints an error message on stderr.
 */
void error(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("error: ", fmt, ap);
    va_end(ap);
}

/**
 * Prints a warning message on stderr.
 */
void warn(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("warning: ", fmt, ap);
    va_end(ap);
}

/**
 * Prints an informative message on stderr.
 */
void info(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("", fmt, ap);
    va_end(ap);
}
