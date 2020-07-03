#include "greatest.h"

#include "vector.h"

TEST vector_test_alloc(void)
{
  vector_t *vp;

  vp = vector_alloc(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  PASS();
}

SUITE(vector_suite)
{
  RUN_TEST(vector_test_alloc);
}
