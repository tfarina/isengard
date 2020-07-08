#ifndef _OPTIMIZATION_H_INCLUDED_
#define _OPTIMIZATION_H_INCLUDED_ 1

/**
 * M_LIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be true.
 *
 * @exp: C expression.
 */

/**
 * M_UNLIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be false.
 *
 * @exp: C expression.
 */

/*
 * Branch prediction hints.
 */
#if __GNUC__ >= 3
# define M_UNLIKELY(exp) __builtin_expect ((exp), 0)
# define M_LIKELY(exp)   __builtin_expect ((exp), 1)
#else
# define M_UNLIKELY(exp) (exp)
# define M_LIKELY(exp)   (exp)
#endif

#endif  /* !defined(_OPTIMIZATION_H_INCLUDED_) */
