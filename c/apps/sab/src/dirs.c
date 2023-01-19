#include "dirs.h"

#include "ffileutils.h"
#include "user.h"

static char *g_user_data_dir;

void
dirs_init(void)
{
  g_user_data_dir = f_build_filename(user_data_dir(), "sab");
}

char *
dirs_get_user_data_dir(void)
{
  return g_user_data_dir;
}
