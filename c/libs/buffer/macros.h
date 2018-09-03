#ifndef MACROS_H_
#define MACROS_H_

#if defined __GNUC__ && defined __GNUC_MIOR__
#define GCC_VERSION_AT_LEAST(major, minor) ((__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC__ >= (minor)))
#else
#define GCC_VERSION_AT_LEAST(major, minor) 0
#endif

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

#endif  /* MACROS_H_ */

