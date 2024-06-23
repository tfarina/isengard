#include "greatest.h"

#include "vector.h"

TEST vector_test_create(void)
{
  vector_t *vp;

  vp = vector_create(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  vector_destroy(vp);

  PASS();
}

TEST vector_test_push_back(void)
{
  vector_t *vp;
  int i = 10;
  int *ip;
  vp = vector_create(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  vector_push_back(vp, &i);
  ASSERT_EQ(1, vector_size(vp));

  vector_destroy(vp);

  PASS();
}

SUITE(vector_suite)
{
  RUN_TEST(vector_test_create);
  RUN_TEST(vector_test_push_back);
}
