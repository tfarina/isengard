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

#endif  /* !defined(_CORE_H_INCLUDED_) */
