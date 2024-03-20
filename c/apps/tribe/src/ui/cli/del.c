#include <stdio.h>
#include <stdlib.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

static char const *progname;

int main(int argc, char **argv) {
  int rc;
  char *dbdir;
  int id;
  int deleted = 0;

  progname = os_path_basename(*argv);

  if (argc != 2) {
    fprintf(stderr, "usage: %s <id>\n", progname);
    return 1;
  }

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    return 1;
  }

  id = atoi(argv[1]);

  rc = ab_delete_contact_v2(id, &deleted);
  if (rc < 0) {
    return 1;
  }

  if (!deleted) {
    fprintf(stderr, "Error: no contact with id %d was found in our database.\n",
            id);
    return 1;
  }

  ab_fini();

  return 0;
}
