#ifndef MACROS_H_
#define MACROS_H_

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#endif  /* MACROS_H_ */
