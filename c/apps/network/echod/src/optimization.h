#ifndef _OPTIMIZATION_H_INCLUDED_
#define _OPTIMIZATION_H_INCLUDED_ 1

/**
 * @file optimization.h
 * @brief Branch prediction hints.
 */

/* Feature-detect __builtin_expect once (Clang/GCC or compilers with
 * __has_builtin)
 */
#if !defined(M_HAS_BUILTIN_EXPECT)
# if defined(__has_builtin)
#  if __has_builtin(__builtin_expect)
#   define M_HAS_BUILTIN_EXPECT 1
#  endif /* __has_builtin(__builtin_expect) */
# endif /* defined(__has_builtin) */

/* Fallback to checking GCC version (__builtin_expect was introduced in
 * GCC version 3.0)
 */
# if !defined(M_HAS_BUILTIN_EXPECT)
#  if defined(__GNUC__) && (__GNUC__ >= 3)
#   define M_HAS_BUILTIN_EXPECT 1
#  endif /* defined(__GNUC__) && (__GNUC__ >= 3) */
# endif /* !defined(M_HAS_BUILTIN_EXPECT) */
#endif /* !defined(M_HAS_BUILTIN_EXPECT) */

/**
 * M_LIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be true.
 *
 * @exp: C expression.
 */
#if !defined(M_LIKELY)
# if defined(M_HAS_BUILTIN_EXPECT)
#  define M_LIKELY(exp) (__builtin_expect(!!(exp), 1))
# else
#  define M_LIKELY(exp) (!!(exp))
# endif
#endif

/**
 * M_UNLIKELY:
 *
 * Indicates to the compiler that the expression @exp is likely to be false.
 *
 * @exp: C expression.
 */
#if !defined(M_UNLIKELY)
# if defined(M_HAS_BUILTIN_EXPECT)
#  define M_UNLIKELY(exp) (__builtin_expect(!!(exp), 0))
# else
#  define M_UNLIKELY(exp) (!!(exp))
# endif
#endif

#endif  /* !defined(_OPTIMIZATION_H_INCLUDED_) */
