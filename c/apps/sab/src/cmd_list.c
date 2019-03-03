#include "commands.h"

#include "ab.h"

int cmd_list(int argc, char **argv) {
  int rc;

  ab_init();

  rc = ab_print_contact_records();

  ab_close();

  return rc;
}
