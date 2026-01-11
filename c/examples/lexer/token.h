#ifndef TOKEN_H_
#define TOKEN_H_

/**
 * Token types.
 */
typedef enum token_kind_e {
  TOK_EOF = 0, /* End-Of-File */

  TOK_PLUS     = '+',
  TOK_LBRACKET = '[',
  TOK_RBRACKET = ']',
  TOK_LBRACE   = '{',
  TOK_RBRACE   = '}',
  TOK_LPAREN   = '(',
  TOK_RPAREN   = ')',

  TOK_END
} token_kind_t;

#endif  /* TOKEN_H_ */
