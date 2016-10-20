#ifndef CLI_DB_H_
#define CLI_DB_H_

#include "third_party/sqlite/amalgamation/sqlite3.h"

sqlite3* db_open(const char* db_file);

void db_close(sqlite3* db);

void sql_stmt_free(sqlite3_stmt* stmt);

#endif  /* CLI_DB_H_ */
