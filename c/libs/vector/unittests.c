#include "greatest.h"

#include "vector.h"

SUITE_EXTERN(vector_suite);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();      /* init & parse command-line args */

  RUN_SUITE(vector_suite);

  GREATEST_MAIN_END();        /* display results */

  return 0;
}
