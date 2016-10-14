#ifndef CLI_DB_H_
#define CLI_DB_H_

#include "deps/sqlite3.h"

sqlite3* db_open(const char* db_file);

#endif  /* CLI_DB_H_ */
