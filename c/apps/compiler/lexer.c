#include "lexer.h"

#include <stdio.h>

void lex(const char *input)
{
  const char *c;

  for (c = input; *c; ++c) {
    switch (*c) {
      case '[':
        printf("<'[', LEFT_BRACKET>\n");
        break;
      case ']':
        printf("<']', RIGHT_BRACKET>\n");
        break;
      case '{':
        printf("<'{', LEFT_BRACE>\n");
        break;
      case '}':
        printf("<'}', RIGHT_BRACE>\n");
        break;
    }
  }
}
