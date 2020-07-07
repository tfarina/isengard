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
 * Report an error message and terminate.
 */
void MK_NORETURN fatal(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("fatal: ", fmt, ap);
    va_end(ap);

    exit(1);
}

/**
 * Report an error message.
 */
void error(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("error: ", fmt, ap);
    va_end(ap);
}

/**
 * Report an warning message.
 */
void warn(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("warning: ", fmt, ap);
    va_end(ap);
}

/**
 * Report an informative message.
 */
void info(char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vmsg("", fmt, ap);
    va_end(ap);
}
