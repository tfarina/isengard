/*
 * strlennull.c
 *
 * Demonstrates the consequences of passing a NULL pointer to strlen().
 *
 * According to the C standard, strlen() expects a pointer to a valid
 * NUL-terminated string. Passing NULL results in undefined behavior.
 *
 * In practice, this often leads to a segmentation fault, as the function
 * attempts to read memory starting at address 0x0. However, this is not
 * guaranteed—undefined behavior means anything can happen, including:
 *
 *   - A segmentation fault (most common)
 *   - Silent incorrect results
 *   - Optimized-away code due to compiler assumptions
 *
 * This example intentionally triggers undefined behavior for educational
 * purposes. Do NOT rely on this behavior in real programs.
 *
 * Related note:
 * Always validate pointers before passing them to functions like strlen(),
 * or use safer abstractions (e.g., GLib's g_strdup()) that handle NULL.
 */
#include <stdlib.h>
#include <string.h>

int
main(void)
{
  char *name = NULL;
  size_t len = 0;

  /* Undefined behavior: strlen() requires a valid string */
  len = strlen(name);

  (void)len; /* prevent unused variable warning */

  return 0;
}
