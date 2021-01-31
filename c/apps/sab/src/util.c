#include "util.h"

#include "dirs.h"
#include "mkdirp.h"
#include "test.h"

int ensure_data_dir(void) {
  char const *data_dir;

  data_dir = dirs_get_user_data_dir();
  if (!test_isdir(data_dir)) {
    f_mkdirp(data_dir, 0700);
  }

  return 1;
}
