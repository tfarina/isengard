#include "db.h"

#include <stdio.h>

#include "sqlite3.h"

static sqlite3 *hdb = NULL;  /* SQLite db handle */

int
db_init(void)
{
	char dbname[] = "abdb.sqlite3";
	static char const create_sql[] =
		"CREATE TABLE IF NOT EXISTS contacts ("
		"  id INTEGER PRIMARY KEY,"
		"  fname TEXT,"
		"  lname TEXT,"
		"  email TEXT"
		");";
	int rc;
	char *err_msg = NULL;

	rc = sqlite3_open(dbname, &hdb);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Failed to open SQLite database %s: %s\n", dbname,
			sqlite3_errmsg(hdb));
		return -1;
	}

    rc = sqlite3_exec(hdb, create_sql, 0, 0, 0);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_exec failed: \n", err_msg);
		sqlite3_free(err_msg);
		return -1;
	}

	return 0;
}
