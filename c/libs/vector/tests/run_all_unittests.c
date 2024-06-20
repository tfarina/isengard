#include "greatest.h"

#include "vector.h"

/* Define a suite, compiled separately.
 */
SUITE_EXTERN(vector_suite);

/* Add all the definitions that need to be in the test runner's main file.
 */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();      /* init and parse command-line arguments */

  RUN_SUITE(vector_suite);

  GREATEST_MAIN_END();        /* display results */

  return 0;
}
