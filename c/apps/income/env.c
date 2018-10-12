#include "env.h"

#include <stdlib.h>

char *get_home_dir(void)
{
  return getenv("HOME");
}
