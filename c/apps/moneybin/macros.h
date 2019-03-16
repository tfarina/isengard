#ifndef MACROS_H_
#define MACROS_H_

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

#define UNUSED __attribute__((unused))

#endif  /* MACROS_H_ */

