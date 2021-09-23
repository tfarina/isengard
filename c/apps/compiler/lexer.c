#include "lexer.h"

#include <stdio.h>

#include "token.h"

static token_kind_t kind;

void lex(const char *input)
{
  const char *c;

  for (c = input; *c; ++c) {
    switch (*c) {
      case '+':
        kind = TOK_PLUS;
        printf("<'+', PLUS>\n");
        break;
      case '[':
        kind = TOK_LBRACKET;
        printf("<'[', LEFT_BRACKET>\n");
        break;
      case ']':
	kind = TOK_RBRACKET;
        printf("<']', RIGHT_BRACKET>\n");
        break;
      case '{':
	kind = TOK_LBRACE;
        printf("<'{', LEFT_BRACE>\n");
        break;
      case '}':
	kind = TOK_RBRACE;
        printf("<'}', RIGHT_BRACE>\n");
        break;
      case '(':
	kind = TOK_LPAREN;
        printf("<'(', LEFT_PAREN>\n");
        break;
      case ')':
	kind = TOK_RPAREN;
        printf("<')', RIGHT_PAREN>\n");
        break;
    }
  }
}
