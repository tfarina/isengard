#ifndef _ATTRIBUTES_H_INCLUDED_
#define _ATTRIBUTES_H_INCLUDED_ 1

#include "compiler_tests.h"

/**
 * MK_NORETURN:
 *
 * Indicates to the compiler that a given function never returns.
 */

#if defined(MK_NORETURN)
#undef MK_NORETURN
#endif
#if __GNUC_PREREQ__(2,8)
#define MK_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define MK_NORETURN __declspec(noreturn)
#else
#define MK_NORETURN
#endif

/**
 * MK_PRINTFLIKE:
 *
 * Tells the compiler to perform `printf` format string checking if the
 * compiler supports it.
 */

#if defined(MK_PRINTFLIKE)
#undef MK_PRINTFLIKE
#endif
#if defined(__GNUC__)
#define MK_PRINTFLIKE(string_index, first_to_check) __attribute__ ((__format__ (__printf__, string_index, first_to_check)))
#else
#define MK_PRINTFLIKE(string_index, first_to_check)
#endif

#endif  /* !defined(_ATTRIBUTES_H_INCLUDED_) */
