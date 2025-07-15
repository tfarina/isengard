#ifndef _BASE_H_INCLUDED_
#define _BASE_H_INCLUDED_ 1

/**
 * M_STRINGIZE:
 *
 * Converts the macro argument @x into a string literal.
 *
 * See https://c-faq.com/ansi/stringize.html for an explanation.
 *
 * @param   x   A macro.
 */
#define M_STRINGIZE_(x) #x
#define M_STRINGIZE(x)  M_STRINGIZE_(x)

/**
 * M_CONCAT:
 *
 * Merges two tokens into one while expanding macros.
 *
 * Refer to https://gnu-c-language-manual.github.io/GNU-C-Language-Manual/Concatenation.html
 * for more information.
 *
 * @param   A   The first token.
 * @param   B   The second token.
 */
#define M_CONCAT_(A, B) A##B
#define M_CONCAT(A, B)  M_CONCAT_(A, B)

#endif  /* !defined(_BASE_H_INCLUDED_) */
