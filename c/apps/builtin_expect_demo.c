/*
 * builtin_expect_demo.c
 *
 * Demonstrates why normalizing expressions to boolean with "!!" is important
 * when using __builtin_expect. Shows the difference between:
 *
 *   #define BAD_LIKELY(x)   __builtin_expect((x), 1)
 *   #define GOOD_LIKELY(x)  __builtin_expect(!!(x), 1)
 *
 * Expected outcome:
 *   For nonzero-but-not-1 values such as -1 or 2, BAD_LIKELY returns the
 *   original value (e.g. -1 or 2), so comparisons like "== 1" will be false
 *   even though the expression is truthy. GOOD_LIKELY returns 0/1 consistently.
 */

#include <stdio.h>

/* Unsafe: does not normalize to boolean (keeps original integer value) */
#define BAD_LIKELY(x)  __builtin_expect((x), 1)
#define BAD_UNLIKELY(x) __builtin_expect((x), 0)

/* Safe: normalize expression to 0/1 so expectation matches boolean semantics */
#define GOOD_LIKELY(x)  __builtin_expect(!!(x), 1)
#define GOOD_UNLIKELY(x) __builtin_expect(!!(x), 0)

static void
print_sep(void)
{
  puts("------------------------------------------------------------");
}

int
main(void)
{
  int test_values[] = {0, 1, -1, 2};
  size_t n = sizeof(test_values) / sizeof(test_values[0]);

  puts("builtin_expect_demo: comparing BAD_LIKELY vs GOOD_LIKELY");
  print_sep();

  for (size_t i = 0; i < n; ++i)
  {
    int v = test_values[i];
    int bad = BAD_LIKELY(v);
    int good = GOOD_LIKELY(v);

    printf("value: %d\n", v);
    printf("  BAD_LIKELY(v)  -> %d\n", bad);
    printf("  GOOD_LIKELY(v) -> %d\n", good);
    printf("  (BAD_LIKELY(v) == 1)  -> %s\n", (bad == 1) ? "true" : "false");
    printf("  (GOOD_LIKELY(v) == 1) -> %s\n", (good == 1) ? "true" : "false");

    /* Show how they behave in boolean contexts (if statements) */
    if (BAD_LIKELY(v))
    {
      printf("  if (BAD_LIKELY(v))  -> branch taken (truthy)\n");
    }
    else
    {
      printf("  if (BAD_LIKELY(v))  -> branch NOT taken (false)\n");
    }
    if (GOOD_LIKELY(v))
    {
      printf("  if (GOOD_LIKELY(v)) -> branch taken (truthy)\n");
    }
    else
    {
      printf("  if (GOOD_LIKELY(v)) -> branch NOT taken (false)\n");
    }

    print_sep();
  }

  /*
   * Additional illustrative check: someone might write code like:
   *
   *   if (BAD_LIKELY(v) == 1)   // intends to test "likely true"
   *
   * For v = -1 this is false even though v is truthy. Using !! fixes this.
   */
  return 0;
}
