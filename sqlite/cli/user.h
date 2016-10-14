#ifndef CLI_USER_H_
#define CLI_USER_H_

#include "third_party/sqlite/amalgamation/sqlite3.h"

/* Makes sure the 'user' table is created if it does not exist yet.
 * Returns 0 on success, -1 otherwise. */
int db_user_create_table(sqlite3* db);

/* Returns 0 if the user does not exists, otherwise returns 1. */
int db_user_exists(sqlite3* db, const char* username);

#endif  /* CLI_USER_H_ */
