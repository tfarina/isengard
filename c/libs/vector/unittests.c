#include "greatest.h"

#include "vector.h"

TEST vector_test_alloc(void) {
  vector_t *vp;

  vp = vector_alloc(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  PASS();
}

SUITE(unittests) {
  RUN_TEST(vector_test_alloc);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
  RUN_SUITE(unittests);
  GREATEST_MAIN_END();        /* display results */
}
