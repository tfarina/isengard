#include "util.h"

const char *f_get_home_dir(void)
{
  return getenv("HOME");
}
