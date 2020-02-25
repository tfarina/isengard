#ifndef HINTS_H_
#define HINTS_H_

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


#if defined(MK_NORETURN)
#undef MK_NORETURN
#endif
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define MK_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define MK_NORETURN __declspec(noreturn)
#else
#define MK_NORETURN
#endif

#if defined(MK_PRINTF)
#undef MK_PRINTF
#endif
#if defined(__GNUC__)
#define MK_PRINTF(string_index, first_to_check) __attribute__ ((__format__ (__printf__, string_index, first_to_check)))
#else
#define MK_PRINTF(string_index, first_to_check)
#endif

#endif  /* HINTS_H_ */
