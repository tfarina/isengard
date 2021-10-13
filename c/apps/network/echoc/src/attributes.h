#ifndef _ATTRIBUTES_H_INCLUDED_
#define _ATTRIBUTES_H_INCLUDED_ 1

/**
 * @file
 * Macro definitions for various function/variable attributes.
 */

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
 * MK_PRINTFLIKE:
 *
 * Tells the compiler to check the arguments in calls to the function for
 * consistency with the `printf` style format string argument (if the
 * compiler supports it).
 *
 * index starts at 1.
 *
 * @param fmtarg      the index of the format argument it should check.
 * @param firstvararg the index of the first variable argument (...) it should check.
 */

#if defined(MK_PRINTFLIKE)
#undef MK_PRINTFLIKE
#endif
#if defined(__GNUC__)
#define MK_PRINTFLIKE(fmtarg, firstvararg) __attribute__ ((__format__ (__printf__, fmtarg, firstvararg)))
#else
#define MK_PRINTFLIKE(fmtarg, firstvararg)
#endif

#endif  /* !defined(_ATTRIBUTES_H_INCLUDED_) */
