#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "db.h"
#include "strutils.h"

static const char dbname[] = "abdb.sqlite3";
static sqlite3 *conn = NULL;

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(sqlite3 *db) {
  int rc;
  sqlite3_stmt *stmt;
  const char sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";

  if ((rc = sqlite3_prepare(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "error preparing create statement: %s\n", sqlite3_errmsg(db));
    db_close(db);
    return -1;
  }

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error creating contacts table: %s\n", sqlite3_errmsg(db));
    db_close(db);
    return -1;
  }

  return 0;
}

ab_contact_t *ab_contact_alloc(void) {
  ab_contact_t *contact = NULL;

  contact = malloc(sizeof(ab_contact_t));
  if (contact == NULL) {
    return NULL;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  return contact;
}

int ab_init(void) {
  int rc;

  /* Do nothing if the database handle has been set. */
  if (conn) {
    return 0;
  }

  rc = db_open(dbname, &conn);
  if (rc) {
    return -1;
  }

  if (_create_tables(conn)) {
    db_close(conn);
    return -1;
  }

  return 0;
}

int ab_close(void) {
  db_close(conn);
  conn = NULL;

  return 0;
}

int ab_add_contact(ab_contact_t *contact) {
  const char *sql;
  sqlite3_stmt *stmt;

  sql = "INSERT INTO contacts (fname, lname, email) VALUES (?1, ?2, ?3);";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(conn));
    db_close(conn);
    return -1;
  }

  sqlite3_bind_text(stmt, 1, contact->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, contact->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into contacts table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

int ab_change_contact(ab_contact_t *contact) {
  const char *sql;
  sqlite3_stmt *stmt;
  int rc;

  sql = "UPDATE contacts SET fname=?2, lname=?3, email=?4 WHERE id=?1;";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_bind_int(stmt, 1, contact->id);

  rc = sqlite3_bind_text(stmt, 2, contact->fname, -1, SQLITE_STATIC);

  if (rc == SQLITE_OK)
    rc = sqlite3_bind_text(stmt, 3, contact->lname, -1, SQLITE_STATIC);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "error binding a value for the contacts table: %s\n",
            sqlite3_errmsg(conn));
  }

  rc = sqlite3_bind_text(stmt, 4, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating contacts table: %s\n", sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

int ab_delete_contact(ab_contact_t *contact) {
  const char *sql;
  sqlite3_stmt *stmt;

  sql = "DELETE FROM contacts WHERE id=?1;";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_bind_int(stmt, 1, contact->id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting contacts from table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

alpm_list_t *ab_get_contact_list(void) {
  const char *sql;
  sqlite3_stmt *stmt;
  alpm_list_t *list = NULL;

  sql = "SELECT * FROM contacts";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(conn));
    return NULL;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    ab_contact_t *contact = ab_contact_alloc();
    contact->id = sqlite3_column_int(stmt, 0);
    contact->fname = f_strdup((const char *)sqlite3_column_text(stmt, 1));
    contact->lname = f_strdup((const char *)sqlite3_column_text(stmt, 2));
    contact->email = f_strdup((const char *)sqlite3_column_text(stmt, 3));
    list = alpm_list_add(list, contact);
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return list;
}

ab_contact_t *ab_get_contact_by_id(alpm_list_t *list, int id) {
  alpm_list_t *i;

  for (i = list; i; i = alpm_list_next(i)) {
    ab_contact_t *contact = (ab_contact_t *)i->data;
    if (contact->id == id) {
      return contact;
    }
  }

  return NULL;
}

int ab_print_contact_records(void) {
  alpm_list_t *list, *i;

  list = ab_get_contact_list();

  for (i = list; i; i = alpm_list_next(i)) {
    ab_contact_t *contact = (ab_contact_t *)i->data;
    printf("%d|%s|%s|%s\n", contact->id, contact->fname, contact->lname, contact->email);
  }

  return 0;
}
