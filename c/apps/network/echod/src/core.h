#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_ 1

/**
 * M_STRINGIFY:
 *
 * Turns the macro @x into a string.
 *
 * @x: macro.
 */
#define M_STRINGIFY_(x) #x
#define M_STRINGIFY(x)  M_STRINGIFY_(x)

#if defined(M_HAS_BUILTIN)
#undef M_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#define M_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#define M_HAS_BUILTIN(builtin) (0)
#endif

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

#endif  /* !defined(_CORE_H_INCLUDED_) */
