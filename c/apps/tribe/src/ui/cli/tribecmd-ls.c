#include "ab.h"
#include "common.h"
#include "dirs.h"
#include "util.h"

int main(int argc, char **argv) {
  int rc;
  char *dbdir;

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    return 1;
  }

  ab_load_contacts();

  rc = print_contact_list(ab_get_contact_list());

  ab_fini();

  return rc;
}
