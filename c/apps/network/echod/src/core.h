#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_ 1

/**
 * M_STRINGIFY:
 * @x: macro.
 *
 * Turns the macro @x into a string.
 **/
#define M_STRINGIFY_(x) #x
#define M_STRINGIFY(x)  M_STRINGIFY_(x)

#endif  /* !defined(_CORE_H_INCLUDED_) */
