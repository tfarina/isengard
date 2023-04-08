#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "os_path.h"
#include "xstring.h"

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
    fprintf(stderr, "Failed to close SQLite database: %s\n", sqlite3_errmsg(hdb));
  } else {
    hdb = NULL;
    /*if (debug) {
      printf("Database closed\n");
    }*/
  }
}

static int _execute_sql(char const *sql) {
  int rc;
  char *err_msg = NULL;

  rc = sqlite3_exec(hdb, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "sqlite3_exec failed: %s\n", err_msg);
    sqlite3_free(err_msg);
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

  dbfile = os_path_join(dbpath, dbname);

  rc = sqlite3_open(dbfile, &hdb);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to open SQLite database %s: %s\n", dbfile, sqlite3_errmsg(hdb));
    free(dbfile);
    sqlite3_close(hdb);
    hdb = NULL;
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
    fprintf(stderr, "Failed to prepare the insert statement: %s\n",
            sqlite3_errmsg(hdb));
    _close_db();
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, update_sql, -1, &update_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the update statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  rc = sqlite3_prepare_v2(hdb, delete_sql, -1, &delete_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the delete statement: %s\n",
            sqlite3_errmsg(hdb));
    return -1;
  }

  return 0;
}

int ab_fini(void) {
  int rc;

  rc = sqlite3_finalize(insert_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the insert statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  insert_stmt = NULL;

  rc = sqlite3_finalize(update_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the update statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  update_stmt = NULL;

  rc = sqlite3_finalize(delete_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the delete statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  delete_stmt = NULL;

  _close_db();
  hdb = NULL;

  return 0;
}

void ab_load_contacts(void) {
  int rc;

  rc = sqlite3_prepare_v2(hdb, select_sql, -1, &select_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare the select statement: %s\n",
            sqlite3_errmsg(hdb));
    return;
  }

  while (sqlite3_step(select_stmt) == SQLITE_ROW) {
    ab_contact_t *contact = ab_contact_alloc();
    if (contact == NULL) {
      continue;
    }

    contact->id = sqlite3_column_int(select_stmt, 0);
    contact->fname = xstrdup((const char *)sqlite3_column_text(select_stmt, 1));
    contact->lname = xstrdup((const char *)sqlite3_column_text(select_stmt, 2));
    contact->email = xstrdup((const char *)sqlite3_column_text(select_stmt, 3));
    contact_list = alpm_list_add(contact_list, contact);
  }

  rc = sqlite3_finalize(select_stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to finalize the select statement: %s\n",
            sqlite3_errmsg(hdb));
  }
  select_stmt = NULL;
}

int _db_insert_contact(ab_contact_t *contact) {
  int rc;
  int errcode = 0;

  rc = sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind fname parameter for insert statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind lname parameter for insert statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind email parameter for insert statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_step(insert_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    errcode = -1;
  }

  sqlite3_reset(insert_stmt);

out:
  return errcode;
}

/*
 * Adds the specified contact to the address book database.
 */
int ab_add_contact(ab_contact_t *contact) {
  contact_list = alpm_list_add(contact_list, contact);

  return _db_insert_contact(contact);
}

int _db_update_contact(ab_contact_t* contact) {
  int rc;
  int errcode = 0;

  rc = sqlite3_bind_int(update_stmt, 1, contact->id);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind id parameter for update statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 2, contact->fname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind fname parameter for update statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 3, contact->lname, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind lname parameter for update statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_bind_text(update_stmt, 4, contact->email, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to bind email parameter for update statement\n");
    errcode = -1;
    goto out;
  }

  rc = sqlite3_step(update_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
	    sqlite3_errmsg(hdb));
    errcode = -1;
  }

  sqlite3_reset(update_stmt);

out:
  return errcode;
}

/*
 * Updates an existing contact in the address book database.
 */
int ab_update_contact(ab_contact_t *contact) {
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
    fprintf(stderr, "Failed to bind id to delete contact: %s\n",
	    sqlite3_errmsg(hdb));
    goto out;
  }

  rc = sqlite3_step(delete_stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "sqlite3_step failed: %s\n",
            sqlite3_errmsg(hdb));
    goto out;
  }

  sqlite3_reset(delete_stmt);

  return 0;

out:
  if (delete_stmt) {
    sqlite3_reset(delete_stmt);
  }

  return -1;
}

/*
 * Deletes a contact from the address book database.
 */
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
