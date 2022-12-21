#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "ffileutils.h"
#include "strutils.h"

static char dbname[] = "abdb.sqlite3";
static sqlite3 *hdb = NULL;  /* SQLite db handle */

static sqlite3_stmt *insert_stmt;
static const char insert_sql[] =
  "INSERT INTO contacts (fname, lname, email) VALUES (?1, ?2, ?3);";

static sqlite3_stmt *update_stmt;
static const char update_sql[] =
  "UPDATE contacts SET fname=?2, lname=?3, email=?4 WHERE id=?1;";

static sqlite3_stmt *delete_stmt;
static const char delete_sql[] =
  "DELETE FROM contacts WHERE id=?1;";

static sqlite3_stmt *select_stmt;
static const char select_sql[] = "SELECT * FROM contacts";

static alpm_list_t *contact_list;

static void _close_db(void) {
  int rc;

  rc = sqlite3_close(hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errmsg(hdb));
  } else {
    hdb = NULL;
    /*if (debug) {
      printf("Database closed\n");
    }*/
  }
}

static int _execute_sql(char const *sql) {
  int rc;
  sqlite3_stmt *sql_stmt;

  rc = sqlite3_prepare_v2(hdb, sql, -1, &sql_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing SQL statement: %s\n", sqlite3_errmsg(hdb));
    _close_db();
    return -1;
  }

  rc = sqlite3_step(sql_stmt);

  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error: step failed: %s\n", sqlite3_errmsg(hdb));
    _close_db();
    return -1;
  }

  return 0;
}

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(void) {
  int rc;
  static const char create_sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";

  rc = _execute_sql(create_sql);

  return rc;
}

int ab_init(char *dbpath) {
  char *dbfile;  /* Database filename */
  int rc;

  /* Do nothing if the database handle has been set. */
  if (hdb) {
    return 0;
  }

  dbfile = f_build_filename(dbpath, dbname);

  rc = sqlite3_open(dbfile, &hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", dbfile, sqlite3_errmsg(hdb));
    free(dbfile);
    sqlite3_close(hdb);
    return -1;
  }
  free(dbfile);

  rc = _create_tables();
  if (rc < 0) {
    _close_db();
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, insert_sql, -1, &insert_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(hdb));
    _close_db();
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, update_sql, -1, &update_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, delete_sql, -1, &delete_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, select_sql, -1, &select_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  return 0;
}

int ab_fini(void) {
  sqlite3_finalize(insert_stmt);
  insert_stmt = NULL;

  sqlite3_finalize(update_stmt);
  update_stmt = NULL;

  sqlite3_finalize(delete_stmt);
  delete_stmt = NULL;

  sqlite3_finalize(select_stmt);
  select_stmt = NULL;

  _close_db();
  hdb = NULL;

  return 0;
}

void ab_load_contacts(void) {
  while (sqlite3_step(select_stmt) == SQLITE_ROW) {
    ab_contact_t *contact = ab_contact_alloc();
    if (contact == NULL) {
      continue;
    }

    contact->id = sqlite3_column_int(select_stmt, 0);
    contact->fname = f_strdup((const char *)sqlite3_column_text(select_stmt, 1));
    contact->lname = f_strdup((const char *)sqlite3_column_text(select_stmt, 2));
    contact->email = f_strdup((const char *)sqlite3_column_text(select_stmt, 3));
    contact_list = alpm_list_add(contact_list, contact);
  }
}

int _db_insert_contact(ab_contact_t *contact) {
  if (sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC) != SQLITE_OK) {
    fprintf(stderr, "error binding arguments");
    return -1;
  }

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into contacts table: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  sqlite3_reset(insert_stmt);

  return 0;
}

int ab_add_contact(ab_contact_t *contact) {
  contact_list = alpm_list_add(contact_list, contact);

  return _db_insert_contact(contact);
}

int _db_update_contact(ab_contact_t* contact) {
  if (sqlite3_bind_int(update_stmt, 1, contact->id) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 2, contact->fname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 3, contact->lname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 4, contact->email, -1, SQLITE_STATIC) != SQLITE_OK) {
    fprintf(stderr, "error binding arguments");
    return -1;
  }

  if (sqlite3_step(update_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating contacts table: %s\n", sqlite3_errmsg(hdb));
    return -1;
  }

  sqlite3_reset(update_stmt);

  return 0;
}

int ab_change_contact(ab_contact_t *contact) {
  return _db_update_contact(contact);
}

static int _ab_contact_cmp(void const *p1, void const *p2) {
  ab_contact_t *c1 = (ab_contact_t *)p1;
  ab_contact_t *c2 = (ab_contact_t *)p2;
  return c1->id == c2->id;
}

static int _db_delete_contact(int id) {
  int rc;

  rc = sqlite3_bind_int(delete_stmt, 1, id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind id to statement: %s\n",
	    sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_step(delete_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to execute delete statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  sqlite3_reset(delete_stmt);

  return 0;
}

int ab_delete_contact(ab_contact_t *contact) {
  void *vc;
  int rc;

  contact_list = alpm_list_remove(contact_list, contact, _ab_contact_cmp, &vc);

  rc = _db_delete_contact(contact->id);

  return rc;
}

alpm_list_t *ab_get_contact_list(void) {
  return contact_list;
}

ab_contact_t *ab_get_contact_by_id(int id) {
  alpm_list_t *item;

  for (item = contact_list; item; item = alpm_list_next(item)) {
    ab_contact_t *contact = (ab_contact_t *)item->data;
    if (contact->id == id) {
      return contact;
    }
  }

  return NULL;
}
