#ifndef _ATTRIBUTES_H_INCLUDED_
#define _ATTRIBUTES_H_INCLUDED_ 1

#include "compiler_tests.h"

/**
 * MK_NORETURN:
 *
 * Indicates to the compiler that a given function never returns.
 *
 * NOTE: It must go before the function declaration, rather than after the
 * declaration, as the MSVC version has to go before the declaration.
 *
 * Example:
 *
 * extern MK_NORETURN func(...);
 *
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
 * MK_PRINTF:
 *
 * Tells the compiler to perform `printf` format string checking if the
 * compiler supports it.
 *
 * index starts at 1.
 *
 * fmtarg is the index of the format argument it should check.
 * firstvararg is the index of the first variable argument (...) it should check.
 */

#if defined(MK_PRINTF)
#undef MK_PRINTF
#endif
#if defined(__GNUC__)
#define MK_PRINTF(fmtarg, firstvararg) __attribute__ ((__format__ (__printf__, fmtarg, firstvararg)))
#else
#define MK_PRINTF(fmtarg, firstvararg)
#endif

#endif  /* !defined(_ATTRIBUTES_H_INCLUDED_) */
