#include <stdio.h>

#include "third_party/libuuid/uuid.h"

int main(void)
{
  uuid_t uuid;
  char uuid_str[37];

  uuid_generate(uuid);
  uuid_unparse(uuid, uuid_str);

  printf("%s\n", uuid_str);

  return 0;
}
