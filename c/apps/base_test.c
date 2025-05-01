#include <stdio.h>

#include "base.h"

#define OP plus

#define MAJOR 6
#define MINOR 9

#define STR_VERSION M_STRINGIZE(MAJOR) "." M_STRINGIZE(MINOR)

int
main(void)
{
  /* This shows why you need a macro indirection to put a macro value
   * in quotes. Applying the # to the macro, will just put the macro name
   * in quotes. So, in order to put the value of the macro in quotes,
   * the macro has to be "expanded" first, then the # has to be applied to
   * the result to stringize it.
   * See https://c-faq.com/ansi/stringize.html for more information.#
   */
  char *macroname = M_STRINGIZE_(OP);
  char *opname = M_STRINGIZE(OP);

  printf("%s\n", macroname); /* This will print OP */
  printf("%s\n", opname);    /* This will print plus */

  printf("%s\n", STR_VERSION);

  printf("%s\n", M_STRINGIZE_(3));

  return 0;
}
