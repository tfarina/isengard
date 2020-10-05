#ifndef _ATTRIBUTES_H_INCLUDED_
#define _ATTRIBUTES_H_INCLUDED_ 1

/*
 * Macro to test if we're using a specific version of gcc or later.
 *
 * Use like this:
 *   #if __GNUC_PREREQ__(2,8)
 *   ... code requiring gcc 2.8 or later ...
 *   #endif
 *
 * Note: only works for GCC 2.0 and later, because __GNUC_MINOR__ was
 * added in 2.0.
 */
#if defined __GNUC__ && defined __GNUC_MINOR__
#define __GNUC_PREREQ__(major, minor) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((major) << 16) + (min))
#else
#define __GNUC_PREREQ__(major, minor) 0
#endif

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
 * MK_PRINTF:
 *
 * Tells the compiler to perform `printf` format string checking if the
 * compiler supports it.
 */

#if defined(MK_PRINTF)
#undef MK_PRINTF
#endif
#if defined(__GNUC__)
#define MK_PRINTF(string_index, first_to_check) __attribute__ ((__format__ (__printf__, string_index, first_to_check)))
#else
#define MK_PRINTF(string_index, first_to_check)
#endif

#endif  /* !defined(_ATTRIBUTES_H_INCLUDED_) */
