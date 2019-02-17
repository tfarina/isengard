#include "ab.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "db.h"
#include "strutils.h"

static const char dbname[] = "abdb.sqlite3";
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

/**
 * Makes sure the 'contacts' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(void) {
  int rc;
  sqlite3_stmt *stmt;
  const char sql[] =
    "CREATE TABLE IF NOT EXISTS contacts ("
    "  id INTEGER PRIMARY KEY,"     /* id */
    "  fname TEXT,"                 /* first name */
    "  lname TEXT,"                 /* last name */
    "  email TEXT"                  /* email */
    ");";

  if (sqlite3_prepare(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing create statement: %s\n", sqlite3_errmsg(conn));
    db_close(conn);
    return -1;
  }

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error creating contacts table: %s\n", sqlite3_errmsg(conn));
    db_close(conn);
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

void ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  contact->fname = fname;
}

void ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  contact->lname = lname;
}

void ab_contact_set_email(ab_contact_t *contact, char const *email) {
  contact->email = email;
}

int ab_init(void) {
  /* Do nothing if the database handle has been set. */
  if (conn) {
    return 0;
  }

  if (sqlite3_open(dbname, &conn) != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", dbname, sqlite3_errmsg(conn));
    sqlite3_close(conn);
    return -1;
  }

  if (_create_tables()) {
    db_close(conn);
    return -1;
  }

  if (sqlite3_prepare(conn, insert_sql, -1, &insert_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(conn));
    db_close(conn);
    return -1;
  }

  if (sqlite3_prepare(conn, update_sql, -1, &update_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare(conn, delete_sql, -1, &delete_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  if (sqlite3_prepare(conn, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  return 0;
}

int ab_close(void) {
  sqlite3_finalize(insert_stmt);
  insert_stmt = NULL;

  sqlite3_finalize(update_stmt);
  update_stmt = NULL;

  sqlite3_finalize(delete_stmt);
  delete_stmt = NULL;

  sqlite3_finalize(select_stmt);
  select_stmt = NULL;

  db_close(conn);
  conn = NULL;

  return 0;
}

int ab_add_contact(ab_contact_t *contact) {
  sqlite3_bind_text(insert_stmt, 1, contact->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(insert_stmt, 2, contact->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(insert_stmt, 3, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into contacts table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(insert_stmt);

  return 0;
}

int ab_change_contact(ab_contact_t *contact) {
  sqlite3_bind_int(update_stmt, 1, contact->id);
  sqlite3_bind_text(update_stmt, 2, contact->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(update_stmt, 3, contact->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(update_stmt, 4, contact->email, -1, SQLITE_STATIC);

  if (sqlite3_step(update_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating contacts table: %s\n", sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(update_stmt);

  return 0;
}

int ab_delete_contact(ab_contact_t *contact) {
  sqlite3_bind_int(delete_stmt, 1, contact->id);

  if (sqlite3_step(delete_stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting contacts from table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_reset(delete_stmt);

  return 0;
}

alpm_list_t *ab_get_contact_list(void) {
  alpm_list_t *list = NULL;

  while (sqlite3_step(select_stmt) == SQLITE_ROW) {
    ab_contact_t *contact = ab_contact_alloc();
    contact->id = sqlite3_column_int(select_stmt, 0);
    contact->fname = f_strdup((const char *)sqlite3_column_text(select_stmt, 1));
    contact->lname = f_strdup((const char *)sqlite3_column_text(select_stmt, 2));
    contact->email = f_strdup((const char *)sqlite3_column_text(select_stmt, 3));
    list = alpm_list_add(list, contact);
  }

  sqlite3_reset(select_stmt);

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
