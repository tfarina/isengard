#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG

void __assertion_failed(char const *assertion, char const *file, int unsigned line) {
  printf("%s:%u: Assertion failed: %s\n", file, line, assertion);

  abort();
}

#endif
