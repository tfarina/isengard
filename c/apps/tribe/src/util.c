#include "util.h"

#include "dirs.h"
#include "mkdirp.h"
#include "os_path.h"

int ensure_data_dir(void) {
  char const *data_dir;
  int retval;

  data_dir = dirs_get_user_data_dir();
  if (!os_path_isdir(data_dir)) {
    retval = f_mkdirp(data_dir, 0700);
    if (retval < 0) {
      return 0;
    }
  }

  return 1;
}
