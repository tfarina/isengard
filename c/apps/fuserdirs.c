#include "fuserdirs.h"

#include "ffileutils.h"
#include "futils.h"

char *f_get_user_config_dir(void)
{
  char *homedir;
  char *configdir;

  homedir = f_get_home_dir();
  configdir = f_build_filename(homedir, ".config");

  return configdir;
}
