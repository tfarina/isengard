#ifndef DB_H_
#define DB_H_

#include "third_party/sqlite/amalgamation/sqlite3.h"

int db_open(const char *filename, sqlite3 **db);

void db_close(sqlite3 *db);

void sql_stmt_free(sqlite3_stmt *stmt);

#endif  /* DB_H_ */
