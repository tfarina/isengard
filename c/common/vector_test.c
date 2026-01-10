#include "utest/utest.h"

#include "vector.h"

UTEST(vector_test, create)
{
  vector_t *vp;

  vp = vector_create(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  vector_destroy(vp);
}

UTEST(vector_test, push_back)
{
  vector_t *vp;
  int i = 10;
  int *ip;
  vp = vector_create(sizeof(int), 2);

  ASSERT_EQ(0, vector_size(vp));

  vector_push_back(vp, &i);
  ASSERT_EQ(1, vector_size(vp));

  vector_destroy(vp);
}
