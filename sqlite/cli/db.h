#ifndef CLI_DB_H_
#define CLI_DB_H_

typedef struct sqlite3 sqlite3;

// Returns a pointer to sqlite3.
sqlite3* db_open(const char* db_file);

#endif  // CLI_DB_H_
