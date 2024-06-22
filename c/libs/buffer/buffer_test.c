#include "greatest.h"

#include "buffer.h"

TEST buffer_test_create(void)
{
  buffer_t *p_buffer;

  p_buffer = buffer_create(sizeof(int));

  ASSERT_EQ(0, buffer_size(p_buffer));

  buffer_destroy(p_buffer);

  PASS();
}

SUITE(buffer_suite)
{
  RUN_TEST(buffer_test_create);
}
