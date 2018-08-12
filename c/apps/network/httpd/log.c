#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void die(const char *msg, ...) {
    va_list args;

    fprintf(stderr, "fatal: ");

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}
