#include "greatest.h"

#include "vector.h"

TEST alloc(void) {
  vector_t *vp;

  vp = vector_alloc(2);

  PASS();
}

SUITE(unittests) {
  RUN_TEST(alloc);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
  RUN_SUITE(unittests);
  GREATEST_MAIN_END();        /* display results */
}
