#ifndef _BASE_H_INCLUDED_
#define _BASE_H_INCLUDED_

/**
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

/* Generic utilities */

/**
 * @brief Computes the number of elements in a statically allocated array.
 *
 * This macro returns the number of elements in an array at compile time.
 * It is intended for use with arrays that are declared locally or globally,
 * not with pointers or dynamically allocated arrays.

 * @code
 * int array[10];
 * size_t array_size = ARRAY_SIZE(array); // array_size will be 10
 * @endcode
 *
 * @warning This macro only works with statically allocated arrays. If used
 * with a pointer (including arrays passed to functions, they decay to a
 * pointer), the result will be incorrect because the `sizeof` operator
 * returns the size of the pointer not the array it may point to.

 * @param[in] array A statically allocated array.
 * @return The number of elements in the array, as a `size_t`.
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

#endif  /* !defined(_BASE_H_INCLUDED_) */
