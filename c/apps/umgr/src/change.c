#include <stdio.h>
#include <stdlib.h>

#include "user.h"

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
  alpm_list_t *list;
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 5) {
    fprintf(stderr, "usage: %s 'ID' 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  ab_init();

  list = ab_get_contact_list();

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(list, id);
  if (!contact) {
    fprintf(stderr, "%s: contact (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return -1;
  }

  contact->fname = argv[2];
  contact->lname = argv[3];
  contact->email = argv[4];

  if (ab_change_contact(contact)) {
    return -1;
  }

  ab_close();

  return 0;
}
