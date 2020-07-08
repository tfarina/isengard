#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_ 1

/**
 * M_STRINGIZE:
 *
 * Turns the macro @x into a string.
 *
 * @x: macro.
 */
#define M_STRINGIZE_(x) #x
#define M_STRINGIZE(x)  M_STRINGIZE_(x)

#endif  /* !defined(_CORE_H_INCLUDED_) */
