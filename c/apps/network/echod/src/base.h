#ifndef _BASE_H_INCLUDED_
#define _BASE_H_INCLUDED_ 1

/**
 * M_STRINGIZE:
 *
 * Converts the macro argument `x` into a string literal.
 *
 * This two-step macro indirection ensures that if `x` is itself a macro,
 * it gets expanded before being converted to a string.
 *
 * @param   x   The argument to be stringified.
 */
#define M_STRINGIZE_(x) #x
#define M_STRINGIZE(x)  M_STRINGIZE_(x)

/**
 * M_CONCAT:
 *
 * Concatenates two tokens into a single token.
 *
 * This macro joins the two arguments into a single token. It's commonly
 * used in macro metaprogramming when generating unique identifiers or
 * symbol names.
 *
 * Internally, it uses a helper macro (M_CONCAT_) to ensure correct
 * expansion when x or y are themselves macros.
 *
 * @param   x   First token to concatenate.
 * @param   y   Second token to concatenate.
 */
#define M_CONCAT_(x, y) x##y
#define M_CONCAT(x, y)  M_CONCAT_(x, y)

#endif  /* !defined(_BASE_H_INCLUDED_) */
