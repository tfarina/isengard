#include "ab.h"
#include "common.h"
#include "dirs.h"
#include "util.h"

int main(int argc, char **argv) {
  int rc;
  int status = 0;
  char *dbdir;
  int num_contacts = 0;
  ab_contact_t *contacts = NULL;

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    status = 1;
    goto out;
  }

  rc = ab_enum_contacts_v2(&num_contacts, &contacts);
  if (rc < 0) {
    status = 1;
    goto out;
  }

  rc = print_contact_list_v2(num_contacts, contacts);

out:
  if (contacts) {
    free(contacts);
    contacts = NULL;
  }

  ab_fini();

  return status;
}
