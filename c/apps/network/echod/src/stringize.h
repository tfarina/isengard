#ifndef _STRINGIZE_H_INCLUDED_
#define _STRINGIZE_H_INCLUDED_ 1

/**
 * M_STRINGIZE:
 *
 * Turns the macro @x into a string.
 *
 * See https://c-faq.com/ansi/stringize.html for an explanation.
 *
 * @param   x   A macro.
 */
#define M_STRINGIZE_(x) #x
#define M_STRINGIZE(x)  M_STRINGIZE_(x)

#endif  /* !defined(_STRINGIZE_H_INCLUDED_) */
