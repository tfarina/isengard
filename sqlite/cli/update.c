#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static int callback(void* data, int argc, char** argv, char** column_name) {
  int i;
  fprintf(stdout, "%s: \n", (const char*)data);
  for (i = 0; i < argc; ++i) {
    printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static sqlite3* db_open(const char* db_file) {
  sqlite3* db;

  if (sqlite3_open(db_file, &db)) {
    fprintf(stderr, "Failed to open database %s: %s\n", "users.db",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  return db;
}

static int db_update(sqlite3* db) {
  const char *sql = "UPDATE user SET email='john@yahoo.com' WHERE uid=1; "
                    "SELECT * FROM user";
  const char* data = "Callback function called";
  if (sqlite3_exec(db, sql, callback, (void*)data, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db;

  db = db_open("users.db");

  if (db_update(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
