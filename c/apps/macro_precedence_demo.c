/*
 * macro_precedence_demo.c
 *
 * Demonstrates why macro expansions should parenthesize their results.
 *
 * Expected output:
 *   BAD_MACRO(1 + 1) gives an unexpected result due to operator precedence.
 *   GOOD_MACRO(1 + 1) gives the expected result.
 */

#include <stdio.h>

/* Intentionally unsafe macro: result not wrapped in parentheses */
#define BAD_MACRO(x) x * 2

/* Safe macro: argument and entire result are parenthesized */
#define GOOD_MACRO(x) ((x) * 2)

int main(void)
{
  /* Simple example (no surprise for simple literal arg) */
  int a = 1 + BAD_MACRO(2);   /* expands to 1 + 2 * 2 => 1 + (2*2) = 5 */
  int b = 1 + GOOD_MACRO(2);  /* expands to 1 + ((2) * 2) = 5 */

  /* Problematic example: argument contains an operator */
  int c = 1 + BAD_MACRO(1 + 1);   /* expands to 1 + 1 + 1 * 2 => parsed as (1 + 1) + (1*2) = 4 */
  int d = 1 + GOOD_MACRO(1 + 1);  /* expands to 1 + ((1 + 1) * 2) = 5 */

  puts("macro_precedence_demo: showing why macro results should be parenthesized\n");

  printf("1 + BAD_MACRO(2)    = %d\n", a);
  printf("1 + GOOD_MACRO(2)   = %d\n", b);
  printf("\n");

  printf("1 + BAD_MACRO(1 + 1)  -> expands unsafely and yields: %d  (unexpected)\n", c);
  printf("1 + GOOD_MACRO(1 + 1) -> expands safely and yields:   %d  (expected)\n", d);

  return 0;
}
