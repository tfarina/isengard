#include "env.h"

#include <stdlib.h>

char *get_homedir(void)
{
  return getenv("HOME");
}
