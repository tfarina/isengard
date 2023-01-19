#include "dirs.h"

#include "os_path.h"
#include "user.h"

static char *g_user_data_dir;

void
dirs_init(void)
{
  g_user_data_dir = os_path_join(user_data_dir(), "sab");
}

char *
dirs_get_user_data_dir(void)
{
  return g_user_data_dir;
}
