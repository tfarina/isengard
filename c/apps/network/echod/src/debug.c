#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
void __assertion_failed( char const *assertion, char const *file, int unsigned line) {
  printf("assertion failed: at \"%s\":%u\n  assertion: \"%s\"\n",
         file, line, assertion);
  abort();
}
#endif
