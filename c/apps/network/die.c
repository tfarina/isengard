#include "die.h"

#include <stdio.h>
#include <stdlib.h>

void die(const char *msg, ...) {
    fprintf(stderr, "fatal: ");
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}
