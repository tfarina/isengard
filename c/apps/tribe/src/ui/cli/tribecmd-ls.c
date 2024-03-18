#include "ab.h"
#include "common.h"
#include "dirs.h"
#include "util.h"

int main(int argc, char **argv) {
  int rc;
  char *dbdir;
  int count = 0;
  ab_contact_t *list = NULL;

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    return 1;
  }

  rc = ab_load_contacts_v2(&count, &list);
  if (rc < 0) {
    return 1;
  }

  rc = print_contact_list_v2(count, list);

  ab_fini();

  return rc;
}
