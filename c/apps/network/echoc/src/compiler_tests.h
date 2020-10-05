/**
 * compiler_tests.h
 */

#ifndef _COMPILER_TESTS_H_INCLUDED_
#define _COMPILER_TESTS_H_INCLUDED_

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

#endif  /* !defined(COMPILER_TESTS_H_INCLUDED_) */
