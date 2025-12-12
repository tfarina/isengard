#ifndef _OPTIMIZATION_H_INCLUDED_
#define _OPTIMIZATION_H_INCLUDED_ 1

/**
 * @file optimization.h
 * @brief Branch prediction hints.
 */

/**
 * M_LIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be true.
 *
 * @exp: C expression.
 */
#if !defined(M_LIKELY)
#if defined(__GNUC__) && (__GNUC__ >= 3)
#define M_LIKELY(exp) __builtin_expect ((exp), 1)
#else
#define M_LIKELY(exp) (exp)
#endif
#endif

/**
 * M_UNLIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be false.
 *
 * @exp: C expression.
 */
#if !defined(M_UNLIKELY)
#if defined(__GNUC__) && (__GNUC__ >= 3)
#define M_UNLIKELY(exp) __builtin_expect ((exp), 0)
#else
#define M_UNLIKELY(exp) (exp)
#endif
#endif

#endif  /* !defined(_OPTIMIZATION_H_INCLUDED_) */
