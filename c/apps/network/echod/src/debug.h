/*
 * debug.h - Debugging macros for echod.
 */
#ifndef _DEBUG_H_INCLUDED_
#define _DEBUG_H_INCLUDED_ 1

/**
 * This must be called with an extra level of parentheses because it's not
 * possible to pass a variable number of arguments to a macro
 * (in portable C89).
 *
 * Example:
 *   DPRINT(("this is a debug message"));
 */

#ifdef NDEBUG

#define DPRINT(msg) ((void) 0)

#else

#define DPRINT(msg)  \
  do {               \
  } while (0)

#endif  /* NDEBUG */

#endif  /* !defined(_DEBUG_H_INCLUDED_) */
