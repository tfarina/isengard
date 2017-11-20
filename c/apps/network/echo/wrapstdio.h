#ifndef WRAPSTDIO_H_
#define WRAPSTDIO_H_

#include <stdio.h>

char *Fgets(char *ptr, int n, FILE *stream);

void Fputs(const char *ptr, FILE *stream);

#endif  // WRAPSTDIO_H_
