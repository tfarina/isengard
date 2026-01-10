/*
 * freenull.c
 *
 * This program demonstrates the defined behavior of free() when passed
 * a NULL pointer.
 *
 * According to the C standard and the free(3) manual page, calling free()
 * with a NULL pointer is explicitly allowed and results in no operation.
 * The call is guaranteed to be safe and does not cause a crash, memory
 * corruption, or undefined behavior.
 *
 * This is useful to know because it allows cleanup code to unconditionally
 * call free() without requiring NULL checks.
 *
 * This file exists for educational and reference purposes only.
 */
#include <stdlib.h>

typedef struct _person
{
  char *name;
} person;

int
main(void)
{
  person *p_person = NULL;

  free(p_person);
  free(NULL);

  return 0;
}
