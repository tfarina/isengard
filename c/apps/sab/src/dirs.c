#include "dirs.h"

#include "ffileutils.h"
#include "futils.h"

static char *user_data_dir;

void
dirs_init(void)
{
  user_data_dir = f_build_filename(f_get_user_data_dir(), "sab");
}

char *
dirs_get_user_data_dir(void)
{
  return user_data_dir;
}
