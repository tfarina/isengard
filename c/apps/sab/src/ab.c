#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "ffileutils.h"
#include "strutils.h"

static char dbname[] = "abdb.sqlite3";
static sqlite3 *conn = NULL;

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

static void _ab_sqlite_disconnect(sqlite3 *db) {
  if (sqlite3_close(db) != SQLITE_OK) {
    fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errmsg(db));
  }
}

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(void) {
  int rc;
  sqlite3_stmt *create_stmt;
  static const char create_sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";

  rc = sqlite3_prepare_v2(conn, create_sql, -1, &create_stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error preparing create statement: %s\n", sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  rc = sqlite3_step(create_stmt);

  sqlite3_finalize(create_stmt);
  create_stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error creating contacts table: %s\n", sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  return 0;
}

int ab_init(char *dbpath) {
  char *dbfile;

  /* Do nothing if the database handle has been set. */
  if (conn) {
    return 0;
  }

  dbfile = f_build_filename(dbpath, dbname);

  if (sqlite3_open(dbfile, &conn) != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", dbfile, sqlite3_errmsg(conn));
    sqlite3_close(conn);
    return -1;
  }

  if (_create_tables()) {
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  if (sqlite3_prepare_v2(conn, insert_sql, -1, &insert_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(conn));
    _ab_sqlite_disconnect(conn);
    return -1;
  }

  if (sqlite3_prepare_v2(conn, update_sql, -1, &update_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare_v2(conn, delete_sql, -1, &delete_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare_v2(conn, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(conn));
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

  _ab_sqlite_disconnect(conn);
  conn = NULL;

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

int ab_add_contact(ab_contact_t *contact) {
  contact_list = alpm_list_add(contact_list, contact);

  if (sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC) != SQLITE_OK) {
    fprintf(stderr, "error binding arguments");
    return -1;
  }

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into contacts table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(insert_stmt);

  return 0;
}

int ab_change_contact(ab_contact_t *contact) {
  if (sqlite3_bind_int(update_stmt, 1, contact->id) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 2, contact->fname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 3, contact->lname, -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(update_stmt, 4, contact->email, -1, SQLITE_STATIC) != SQLITE_OK) {
    fprintf(stderr, "error binding arguments");
    return -1;
  }

  if (sqlite3_step(update_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating contacts table: %s\n", sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(update_stmt);

  return 0;
}

static int _ab_contact_cmp(void const *p1, void const *p2) {
  ab_contact_t *c1 = (ab_contact_t *)p1;
  ab_contact_t *c2 = (ab_contact_t *)p2;
  return c1->id == c2->id;
}

int ab_delete_contact(ab_contact_t *contact) {
  void *vc;

  contact_list = alpm_list_remove(contact_list, contact, _ab_contact_cmp, &vc);

  if (sqlite3_bind_int(delete_stmt, 1, contact->id) != SQLITE_OK) {
    fprintf(stderr, "error binding arguments");
    return -1;
  }

  if (sqlite3_step(delete_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting contacts from table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(delete_stmt);

  return 0;
}

alpm_list_t *ab_get_contact_list(void) {
  return contact_list;
}

ab_contact_t *ab_get_contact_by_id(int id) {
  alpm_list_t *lp;

  for (lp = contact_list; lp; lp = alpm_list_next(lp)) {
    ab_contact_t *contact = (ab_contact_t *)lp->data;
    if (contact->id == id) {
      return contact;
    }
  }

  return NULL;
}
