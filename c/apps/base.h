#ifndef _BASE_H_INCLUDED_
#define _BASE_H_INCLUDED_ 1

/**
 * M_STRINGIZE:
 *
 * Converts the macro argument @x into a string literal.
 *
 * This two-step macro indirection ensures that if @x is itself a macro,
 * it gets expanded before being converted to a string.
 *
 * @param   x   The argument to be stringified.
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
 * @param   x   The first token.
 * @param   y   The second token.
 */
#define M_CONCAT_(x, y) x##y
#define M_CONCAT(x, y)  M_CONCAT_(x, y)

#endif  /* !defined(_BASE_H_INCLUDED_) */
