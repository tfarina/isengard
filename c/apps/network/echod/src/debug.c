#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG

void __assertion_failed(char const *assertion, char const *file, int line) {
  fprintf(stderr, "%s:%d: Assertion failed: %s\n", file, line, assertion);
  abort();
}

#endif
