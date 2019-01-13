#ifndef USER_H_
#define USER_H_

#include "third_party/sqlite/amalgamation/sqlite3.h"

/**
 * Makes sure the 'user' table is created if it does not exist yet.
 * Returns 0 on success, -1 otherwise.
 */
int user_init_database(sqlite3* db);

/**
 * Returns 0 if the user does not exists, otherwise returns 1.
 */
int user_exists(sqlite3* db, const char* email);

int user_add(sqlite3* db,
             const char* fname,
             const char* lname,
             const char* email);

int user_change(sqlite3* db,
                const char *fname,
                const char *email);

int user_delete(sqlite3 *db, const char *email);

/**
 * Lists records from the user table.
 */
int user_list(void);

#endif  /* USER_H_ */
