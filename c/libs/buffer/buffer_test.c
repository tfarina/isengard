#include "utest.h"

#include "buffer.h"

UTEST(buffer_test, create)
{
  buffer_t *p_buffer;

  p_buffer = buffer_create(sizeof(int));

  ASSERT_EQ(0, buffer_size(p_buffer));

  buffer_destroy(p_buffer);
}
