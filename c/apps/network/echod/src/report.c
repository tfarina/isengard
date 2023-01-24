#include "report.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void fatal_exit(int const code, char const *format, ...) {
  va_list ap;

  va_start(ap, format);
  fprintf(stderr, "error: ");
  vfprintf(stderr, format, ap);
  fputc('\n', stderr);
  va_end(ap);

  exit(code);
}
