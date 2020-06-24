#include <stdio.h>
#include <stdlib.h>

#include "ab.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

// static int callback(void* data, int argc, char** argv, char** column_name) {
//   int i;
//   //fprintf(stdout, "%s: \n", (const char*)data);
//   for (i = 0; i < argc; ++i) {
//     printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
//   }
//   printf("\n");
//   return 0;
// }

int main(int argc, char **argv) {
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 5) {
    fprintf(stderr, "usage: %s 'ID' 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  ab_init();

  ab_load_contacts();

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(id);
  if (!contact) {
    fprintf(stderr, "%s: contact (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return -1;
  }

  ab_contact_set_first_name(contact, argv[2]);
  ab_contact_set_last_name(contact, argv[3]);
  ab_contact_set_email(contact, argv[4]);

  if (ab_change_contact(contact)) {
    return -1;
  }

  ab_close();

  return 0;
}
