#include "commands.h"

#include "ab.h"
#include "common.h"
#include "util.h"

int cmd_list(int argc, char **argv) {
  int rc;
  char *dbdir;

  dbdir = ensure_data_dir();

  ab_init(dbdir);

  ab_load_contacts();

  rc = print_contact_list(ab_get_contact_list());

  ab_fini();

  return rc;
}
