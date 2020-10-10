#include "util.h"

#include "ffileutils.h"
#include "futils.h"
#include "mkdirp.h"
#include "test.h"

char *ensure_data_dir(void) {
  char *dbdir;

  dbdir = f_build_filename(f_get_user_data_dir(), "sab");
  if (!test_isdir(dbdir)) {
    f_mkdirp(dbdir, 0700);
  }

  return dbdir;
}
